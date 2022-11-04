// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*!
 * @file PKIDH.cpp
 */

#include "PKIDH.h"
#include "PKIIdentityHandle.h"
#include <fastrtps/log/Log.h>
#include <fastrtps/rtps/messages/CDRMessage.h>
#include <fastrtps/rtps/builtin/data/ParticipantProxyData.h>

#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/obj_mac.h>

#include <cassert>
#include <algorithm>

#define S1(x) #x
#define S2(x) S1(x)
#define LOCATION " (" __FILE__ ":" S2(__LINE__) ")"
#define _SecurityException_(str) SecurityException(std::string(str) + LOCATION)

using namespace eprosima::fastrtps;
using namespace ::rtps;
using namespace ::security;

static size_t alignment(size_t current_alignment, size_t dataSize) { return (dataSize - (current_alignment % dataSize)) & (dataSize-1);}

size_t BN_serialized_size(const BIGNUM* bn, size_t current_alignment = 0)
{
    size_t initial_alignment = current_alignment;

    current_alignment += 4 + alignment(current_alignment, 4) + BN_num_bytes(bn);

    return current_alignment - initial_alignment;
}

unsigned char* BN_serialize(const BIGNUM* bn, const unsigned char* orig_pointer, unsigned char* current_pointer)
{
    assert(bn);
    assert(orig_pointer);
    assert(current_pointer);

    size_t align = alignment(current_pointer - orig_pointer, 4);
    unsigned char* pointer = current_pointer + align;
    int32_t len = (int32_t)BN_num_bytes(bn);

#if __BIG_ENDIAN__
        pointer[0] = ((char*)&len)[0];
        pointer[1] = ((char*)&len)[1];
        pointer[2] = ((char*)&len)[2];
        pointer[3] = ((char*)&len)[3];
#else
        pointer[0] = ((char*)&len)[3];
        pointer[1] = ((char*)&len)[2];
        pointer[2] = ((char*)&len)[1];
        pointer[3] = ((char*)&len)[0];
#endif

    pointer += 4;

    if(BN_bn2bin(bn, pointer) == len)
        return pointer + len;

    return nullptr;
}

const unsigned char* BN_deserialize(BIGNUM** bn, const unsigned char* orig_pointer, const unsigned char* current_pointer,
        SecurityException& exception)
{
    assert(bn);
    assert(orig_pointer);
    assert(current_pointer);

    size_t align = alignment(current_pointer - orig_pointer, 4);
    const unsigned char* pointer = current_pointer + align;
    uint32_t length = 0;

#if __BIG_ENDIAN__
        ((char*)&length)[0] = pointer[0];
        ((char*)&length)[1] = pointer[1];
        ((char*)&length)[2] = pointer[2];
        ((char*)&length)[3] = pointer[3];
#else
        ((char*)&length)[0] = pointer[3];
        ((char*)&length)[1] = pointer[2];
        ((char*)&length)[2] = pointer[1];
        ((char*)&length)[3] = pointer[0];
#endif

    pointer += 4;

    BIGNUM *bnn = BN_new();

    if(bnn != nullptr)
    {
        if(BN_bin2bn(pointer, length, bnn) !=  nullptr)
        {
            *bn = bnn;
            return pointer + length;
        }
        else
            exception = _SecurityException_("Cannot deserialize DH");

        BN_free(bnn);
    }
    else
        exception = _SecurityException_("OpenSSL library cannot create bignum");

    return nullptr;
}

bool get_signature_algorithm(X509* certificate, std::string& signature_algorithm, SecurityException& exception)
{
    bool returnedValue = false;
    BUF_MEM* ptr = nullptr;
    X509_ALGOR* sigalg = nullptr;
    ASN1_BIT_STRING* sig = nullptr;

    BIO *out = BIO_new(BIO_s_mem());

    if(out != nullptr)
    {
        X509_get0_signature(&sig, &sigalg, certificate);

        if(sigalg != nullptr)
        {
            if(i2a_ASN1_OBJECT(out, sigalg->algorithm) > 0)
            {
                BIO_get_mem_ptr(out, &ptr);

                if(ptr != nullptr)
                {
                    if(strncmp(ptr->data, "ecdsa-with-SHA256", ptr->length) == 0)
                    {
                        signature_algorithm = ECDSA_SHA256;
                        returnedValue = true;
                    }
                }
                else
                    exception = _SecurityException_("OpenSSL library cannot retrieve mem ptr");
            }
        }
        else
            exception = _SecurityException_("OpenSSL library cannot write cert");

        BIO_free(out);
    }
    else
        exception = _SecurityException_("OpenSSL library cannot allocate mem");

    return returnedValue;
}

// Auxiliary functions
X509_STORE* load_identity_ca(const std::string& identity_ca, bool& there_are_crls,
        std::string& ca_sn, std::string& ca_algo, SecurityException& exception)
{
    X509_STORE* store = X509_STORE_new();

    if(store != nullptr)
    {
        if(identity_ca.size() >= 7 && identity_ca.compare(0, 7, "file://") == 0)
        {
            BIO* in = BIO_new(BIO_s_file());

            if(in != nullptr)
            {
                if(BIO_read_filename(in, identity_ca.substr(7).c_str()) > 0)
                {
                    STACK_OF(X509_INFO)* inf = PEM_X509_INFO_read_bio(in, NULL, NULL, NULL);

                    if(inf != nullptr)
                    {
                        int i, count = 0;
                        there_are_crls = false;

                        for (i = 0; i < sk_X509_INFO_num(inf); i++)
                        {
                            X509_INFO* itmp = sk_X509_INFO_value(inf, i);

                            if (itmp->x509)
                            {
                                // Retrieve subject name for future use.
                                if(ca_sn.empty())
                                {
                                    X509_NAME* ca_subject_name = X509_get_subject_name(itmp->x509);
                                    assert(ca_subject_name != nullptr);
                                    char* ca_subject_name_str = X509_NAME_oneline(ca_subject_name, 0, 0);
                                    assert(ca_subject_name_str != nullptr);
                                    ca_sn = ca_subject_name_str;
                                    CRYPTO_free(ca_subject_name_str);
                                }

                                // Retrieve signature algorithm
                                if(ca_algo.empty())
                                {
                                    if(get_signature_algorithm(itmp->x509, ca_algo, exception))
                                    {
                                        X509_STORE_add_cert(store, itmp->x509);
                                        count++;
                                    }
                                }
                                else
                                {
                                    X509_STORE_add_cert(store, itmp->x509);
                                    count++;
                                }
                            }
                            if (itmp->crl)
                            {
                                X509_STORE_add_crl(store, itmp->crl);
                                there_are_crls = true;
                            }
                        }

                        if(count > 0)
                        {
                            sk_X509_INFO_pop_free(inf, X509_INFO_free);
                            BIO_free(in);

                            return store;
                        }
                    }
                    else
                        exception = _SecurityException_(std::string("OpenSSL library cannot read X509 info in file ") + identity_ca.substr(7));

                    sk_X509_INFO_pop_free(inf, X509_INFO_free);
                }
                else
                    exception = _SecurityException_(std::string("OpenSSL library cannot read file ") + identity_ca.substr(7));

                BIO_free(in);
            }
            else
                exception = _SecurityException_("OpenSSL library cannot allocate file");
        }

        X509_STORE_free(store);
    }
    else
        exception = _SecurityException_("Creation of X509 storage");

    return nullptr;
}

X509* load_certificate(const std::string& identity_cert, SecurityException& exception)
{
    X509* returnedValue = nullptr;

    if(identity_cert.size() >= 7 && identity_cert.compare(0, 7, "file://") == 0)
    {
        BIO* in = BIO_new(BIO_s_file());

        if(in != nullptr)
        {
            if(BIO_read_filename(in, identity_cert.substr(7).c_str()) > 0)
            {
                returnedValue = PEM_read_bio_X509_AUX(in, NULL, NULL, NULL);
            }
            else
                exception = _SecurityException_(std::string("OpenSSL library cannot read file ") + identity_cert.substr(7));

            BIO_free(in);
        }
        else
            exception = _SecurityException_("OpenSSL library cannot allocate file");
    }

    return returnedValue;
}

X509* load_certificate(const std::vector<uint8_t>& data)
{
    X509* returnedValue = nullptr;

    if(data.size() <= static_cast<size_t>(std::numeric_limits<int>::max()))
    {
        BIO* cid = BIO_new_mem_buf(data.data(), static_cast<int>(data.size()));

        if(cid != nullptr)
        {
            returnedValue = PEM_read_bio_X509_AUX(cid, NULL, NULL, NULL);
            BIO_free(cid);
        }
    }

    return returnedValue;
}

bool verify_certificate(X509_STORE* store, X509* cert, const bool there_are_crls)
{
    assert(store);
    assert(cert);

    bool returnedValue = false;

    X509_STORE_CTX ctx;

    unsigned long flags = there_are_crls ? X509_V_FLAG_CRL_CHECK : 0;
    X509_STORE_CTX_init(&ctx, store, cert, NULL);
    X509_STORE_CTX_set_flags(&ctx, flags | X509_V_FLAG_X509_STRICT |
            X509_V_FLAG_CHECK_SS_SIGNATURE | X509_V_FLAG_POLICY_CHECK);

    if(X509_verify_cert(&ctx) > 0 && X509_STORE_CTX_get_error(&ctx) == X509_V_OK)
        returnedValue = true;
    else
        logWarning(SECURITY_AUTHENTICATION, "Invalidation error of certificate  (" << X509_STORE_CTX_get_error(&ctx) << ")");

    X509_STORE_CTX_cleanup(&ctx);

    return returnedValue;
}

int private_key_password_callback(char* buf, int bufsize, int /*verify*/, const char* password)
{
    assert(password != nullptr);

    int returnedValue = static_cast<int>(strlen(password));

    if(returnedValue > bufsize)
        returnedValue = bufsize;

    memcpy(buf, password, returnedValue);
    return returnedValue;
}

EVP_PKEY* load_private_key(X509* certificate, const std::string& file, const std::string& password,
        SecurityException& exception)
{
    EVP_PKEY* returnedValue = nullptr;
    if(file.size() >= 7 && file.compare(0, 7, "file://") == 0)
    {
        BIO* in = BIO_new(BIO_s_file());

        if(in != nullptr)
        {
            if(BIO_read_filename(in, file.substr(7).c_str()) > 0)
            {
                returnedValue = PEM_read_bio_PrivateKey(in, NULL, (pem_password_cb*)private_key_password_callback, (void*)password.c_str());

                // Verify private key.
                if(!X509_check_private_key(certificate, returnedValue))
                {
                    exception = _SecurityException_(std::string("Error verifying private key ") + file.substr(7));
                    EVP_PKEY_free(returnedValue);
                    returnedValue = nullptr;
                }
            }
            else
                exception = _SecurityException_(std::string("OpenSSL library cannot read file ") + file.substr(7));

            BIO_free(in);
        }
        else
            exception = _SecurityException_("OpenSSL library cannot allocate file");
    }

    return returnedValue;
}

bool store_certificate_in_buffer(X509* certificate, BUF_MEM** ptr, SecurityException& exception)
{
    bool returnedValue = false;

    BIO *out = BIO_new(BIO_s_mem());

    if(out != nullptr)
    {
        if(PEM_write_bio_X509(out, certificate) > 0 )
        {
            BIO_get_mem_ptr(out, ptr);

            if(*ptr != nullptr)
            {
                (void)BIO_set_close(out, BIO_NOCLOSE);
                returnedValue = true;
            }
            else
                exception = _SecurityException_("OpenSSL library cannot retrieve mem ptr");
        }
        else
            exception = _SecurityException_("OpenSSL library cannot write cert");

        BIO_free(out);
    }
    else
        exception = _SecurityException_("OpenSSL library cannot allocate mem");

    return returnedValue;
}

bool sign_sha256(EVP_PKEY* private_key, const unsigned char* data, const size_t data_length,
        std::vector<uint8_t>& signature, SecurityException& exception)
{
    assert(private_key);
    assert(data);

    bool returnedValue = false;
    EVP_MD_CTX ctx;
    EVP_MD_CTX_init(&ctx);

    if(EVP_DigestSignInit(&ctx, NULL, EVP_sha256(), NULL, private_key) == 1)
    {
        if(EVP_DigestSignUpdate(&ctx, data, data_length) == 1)
        {
            size_t length = 0;
            if(EVP_DigestSignFinal(&ctx, NULL, &length) == 1 && length > 0)
            {
                signature.resize(length);

                if(EVP_DigestSignFinal(&ctx, signature.data(), &length) ==  1)
                {
                    signature.resize(length);
                    returnedValue = true;
                }
                else
                    exception = _SecurityException_(std::string("Cannot finish signature (") + std::to_string(ERR_get_error()) + std::string(")"));
            }
            else
                exception = _SecurityException_(std::string("Cannot retrieve signature length (") + std::to_string(ERR_get_error()) + std::string(")"));
        }
        else
            exception = _SecurityException_(std::string("Cannot sign data (") + std::to_string(ERR_get_error()) + std::string(")"));
    }
    else
        exception = _SecurityException_(std::string("Cannot init signature (") + std::to_string(ERR_get_error()) + std::string(")"));

    EVP_MD_CTX_cleanup(&ctx);

    return returnedValue;
}

bool check_sign_sha256(X509* certificate, const unsigned char* data, const size_t data_length,
        const std::vector<uint8_t>& signature, SecurityException& exception)
{
    assert(certificate);
    assert(data);

    bool returnedValue = false;

    EVP_MD_CTX ctx;
    EVP_MD_CTX_init(&ctx);

    EVP_PKEY* pubkey = X509_get_pubkey(certificate);
    
    if(pubkey != nullptr)
    {
        if(EVP_DigestVerifyInit(&ctx, NULL, EVP_sha256(), NULL, pubkey) == 1)
        {
            if(EVP_DigestVerifyUpdate(&ctx, data, data_length) == 1)
            {
                if(EVP_DigestVerifyFinal(&ctx, signature.data(), signature.size()) == 1)
                    returnedValue = true;
                else
                    logWarning(SECURITY_AUTHENTICATION, "Signature verification error (" << ERR_get_error() << ")");
            }
            else
                exception = _SecurityException_(std::string("Cannot update signature check (") + std::to_string(ERR_get_error()) + std::string(")"));

        }
        else
            exception = _SecurityException_(std::string("Cannot init signature check (") + std::to_string(ERR_get_error()) + std::string(")"));

        EVP_PKEY_free(pubkey);
    }
    else
        exception = _SecurityException_("Cannot get public key from certificate");

    EVP_MD_CTX_cleanup(&ctx);

    return returnedValue;
}


X509_CRL* load_crl(const std::string& identity_crl, SecurityException& exception)
{
    X509_CRL* returnedValue = nullptr;

    if(identity_crl.size() >= 7 && identity_crl.compare(0, 7, "file://") == 0)
    {
        BIO *in = BIO_new(BIO_s_file());

        if(in != nullptr)
        {
            if(BIO_read_filename(in, identity_crl.substr(7).c_str()) > 0)
            {
                returnedValue = PEM_read_bio_X509_CRL(in, NULL, NULL, NULL);
            }
            else
                exception = _SecurityException_(std::string("OpenSSL library cannot read file ") + identity_crl.substr(7));

            BIO_free(in);
        }
        else
            exception = _SecurityException_("OpenSSL library cannot allocate file");
    }

    return returnedValue;
}

bool adjust_participant_key(X509* cert, const GUID_t& candidate_participant_key,
        GUID_t& adjusted_participant_key, SecurityException& exception)
{
    assert(cert != nullptr);

    X509_NAME* cert_sn = X509_get_subject_name(cert);
    assert(cert_sn != nullptr);

    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned int length = 0;

    if(!X509_NAME_digest(cert_sn, EVP_sha256(), md, &length) || length != SHA256_DIGEST_LENGTH)
    {
        exception = _SecurityException_("OpenSSL library cannot hash sha256");
        return false;
    }

    adjusted_participant_key.guidPrefix.value[0] = 0x80 | (md[0] >> 1);
    adjusted_participant_key.guidPrefix.value[1] = (md[0] << 7) | (md[1] >> 1);
    adjusted_participant_key.guidPrefix.value[2] = (md[1] << 7) | (md[2] >> 1);
    adjusted_participant_key.guidPrefix.value[3] = (md[2] << 7) | (md[3] >> 1);
    adjusted_participant_key.guidPrefix.value[4] = (md[3] << 7) | (md[4] >> 1);
    adjusted_participant_key.guidPrefix.value[5] = (md[4] << 7) | (md[5] >> 1);

    unsigned char key[16] = {
        candidate_participant_key.guidPrefix.value[0],
        candidate_participant_key.guidPrefix.value[1],
        candidate_participant_key.guidPrefix.value[2],
        candidate_participant_key.guidPrefix.value[3],
        candidate_participant_key.guidPrefix.value[4],
        candidate_participant_key.guidPrefix.value[5],
        candidate_participant_key.guidPrefix.value[6],
        candidate_participant_key.guidPrefix.value[7],
        candidate_participant_key.guidPrefix.value[8],
        candidate_participant_key.guidPrefix.value[9],
        candidate_participant_key.guidPrefix.value[10],
        candidate_participant_key.guidPrefix.value[11],
        candidate_participant_key.entityId.value[0],
        candidate_participant_key.entityId.value[1],
        candidate_participant_key.entityId.value[2],
        candidate_participant_key.entityId.value[3]
    };

    if(!EVP_Digest(&key, 16, md, NULL, EVP_sha256(), NULL))
    {
        exception = _SecurityException_("OpenSSL library cannot hash sha256");
        return false;
    }

    adjusted_participant_key.guidPrefix.value[6] = md[0];
    adjusted_participant_key.guidPrefix.value[7] = md[1];
    adjusted_participant_key.guidPrefix.value[8] = md[2];
    adjusted_participant_key.guidPrefix.value[9] = md[3];
    adjusted_participant_key.guidPrefix.value[10] = md[4];
    adjusted_participant_key.guidPrefix.value[11] = md[5];

    adjusted_participant_key.entityId.value[0] = candidate_participant_key.entityId.value[0];
    adjusted_participant_key.entityId.value[1] = candidate_participant_key.entityId.value[1];
    adjusted_participant_key.entityId.value[2] = candidate_participant_key.entityId.value[2];
    adjusted_participant_key.entityId.value[3] = candidate_participant_key.entityId.value[3];

    return true;
}

int get_dh_type(const std::string& algorithm)
{
    if(algorithm.compare(DH_2048_256) == 0)
        return EVP_PKEY_DH;
    else if(algorithm.compare(ECDH_prime256v1) == 0)
        return EVP_PKEY_EC;

    return 0;
}

EVP_PKEY* generate_dh_key(int type, SecurityException& exception)
{
    EVP_PKEY* keys = nullptr;
    EVP_PKEY* params = EVP_PKEY_new();
    EVP_PKEY_CTX* context = nullptr;

    if(params != nullptr)
    {
        DH* dh = nullptr;

        switch(type)
        {
            case EVP_PKEY_DH:
                dh = DH_get_2048_256();
                break;
        };

        if(dh != nullptr)
        {
            int ret = EVP_PKEY_set1_DH(params, dh);

            if(ret)
            {
                if((context = EVP_PKEY_CTX_new(params, NULL)) != nullptr)
                {
                    if(EVP_PKEY_keygen_init(context))
                    {
                        if(!EVP_PKEY_keygen(context, &keys))
                            exception = _SecurityException_("Cannot generate EVP key");
                    }
                    else
                        exception = _SecurityException_("Cannot init EVP key");

                    EVP_PKEY_CTX_free(context);
                }
                else
                    exception = _SecurityException_("Cannot create EVP context");
            }
            else
                exception = _SecurityException_("Cannot set default paremeters");

            DH_free(dh);
        }

        EVP_PKEY_free(params);
    }
    else
        exception = _SecurityException_("Cannot allocate EVP parameters");

    return keys;
}

bool store_dh_public_key(EVP_PKEY* dhkey, std::vector<uint8_t>& buffer,
        SecurityException& exception)
{
    bool returnedValue = false;
    DH* dh = EVP_PKEY_get1_DH(dhkey);

    if(dh != nullptr)
    {
        const BIGNUM* p = dh->p;
        const BIGNUM* g = dh->g;
        const BIGNUM* pub_key = dh->pub_key;
        size_t len = BN_serialized_size(p);
        len += BN_serialized_size(g);
        len += BN_serialized_size(pub_key);
        buffer.resize(len);
        unsigned char* pointer = buffer.data();
        if((pointer = BN_serialize(p, buffer.data(), pointer)) != nullptr)
        {
            if((pointer = BN_serialize(g, buffer.data(), pointer)) != nullptr)
            {
                if((pointer = BN_serialize(pub_key, buffer.data(), pointer)) != nullptr)
                {
                    returnedValue =  true;
                }
                else
                    exception = _SecurityException_("Cannot serialize public key");
            }
            else
                exception = _SecurityException_("Cannot serialize g");
        }
        else
            exception = _SecurityException_("Cannot serialize p");

        DH_free(dh);
    }
    else
        exception = _SecurityException_("OpenSSL library doesn't retrieve DH");

    return returnedValue;
}

EVP_PKEY* generate_dh_peer_key(const std::vector<uint8_t>& buffer, SecurityException& exception)
{
    DH* dh = DH_new();

    if(dh != nullptr)
    {
        const unsigned char* pointer = buffer.data();

        if((pointer = BN_deserialize(&dh->p, buffer.data(), pointer, exception)) != nullptr)
        {
            if((pointer = BN_deserialize(&dh->g, buffer.data(), pointer, exception)) != nullptr)
            {
                if((pointer = BN_deserialize(&dh->pub_key, buffer.data(), pointer, exception)) != nullptr)
                {
                    EVP_PKEY* key = EVP_PKEY_new();

                    if(key != nullptr)
                    {
                        if(EVP_PKEY_assign_DH(key, dh) > 0)
                        {
                            return key;
                        }
                        else
                            exception = _SecurityException_("OpenSSL library cannot set dh in pkey");

                        EVP_PKEY_free(key);
                    }
                    else
                        exception = _SecurityException_("OpenSSL library cannot create pkey");
                }
                else
                    exception = _SecurityException_("Cannot deserialize public key");
            }
            else
                exception = _SecurityException_("Cannot deserialize g");
        }
        else
            exception = _SecurityException_("Cannot deserialize p");

        DH_free(dh);
    }
    else
        exception = _SecurityException_("OpenSSL library cannot create dh");

    return nullptr;
}

bool generate_challenge(std::vector<uint8_t>& vector, SecurityException& exception)
{
    bool returnedValue = false;
    BIGNUM bn;
    BN_init(&bn);

    if(BN_rand(&bn, 512, 0 /*BN_RAND_TOP_ONE*/, 0 /*BN_RAND_BOTTOM_ANY*/))
    {
        int len = BN_num_bytes(&bn);
        vector.resize(len);

        if(BN_bn2bin(&bn, vector.data()) == len)
            returnedValue = true;
        else
            exception = _SecurityException_("OpenSSL library cannot store challenge");
    }

    BN_clear_free(&bn);

    return returnedValue;
}

SharedSecretHandle* generate_sharedsecret(EVP_PKEY* private_key, EVP_PKEY* public_key,
        SecurityException& exception)
{
    assert(private_key);
    assert(public_key);

    SharedSecretHandle* handle = nullptr;
    EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(private_key, NULL);

    if(ctx != nullptr)
    {
        if(EVP_PKEY_derive_init(ctx) > 0)
        {
            if(EVP_PKEY_derive_set_peer(ctx, public_key) > 0)
            {
                size_t length = 0;
                if(EVP_PKEY_derive(ctx, NULL, &length) > 0)
                {
                    SharedSecret::BinaryData data;
                    data.name("SharedSecret");
                    data.value().resize(length);
                    
                    if(EVP_PKEY_derive(ctx, data.value().data(), &length) > 0)
                    {
                        data.value().resize(length);
                        handle = new SharedSecretHandle();
                        (*handle)->data_.push_back(std::move(data));
                    }
                    else
                        exception = _SecurityException_("OpenSSL library cannot get derive");
                }
                else
                    exception = _SecurityException_("OpenSSL library cannot get length");
            }
            else
                exception = _SecurityException_("OpenSSL library cannot set peer");
        }
        else
            exception = _SecurityException_("OpenSSL library cannot init derive");

        EVP_PKEY_CTX_free(ctx);
    }
    else
        exception = _SecurityException_("OpenSSL library cannot allocate context");

    return handle;
}

bool generate_identity_token(PKIIdentityHandle& handle)
{
    Property property;
    IdentityToken& token = handle->identity_token_;
    token.class_id("DDS:Auth:PKI-DH:1.0");

    X509_NAME* cert_sn = X509_get_subject_name(handle->cert_);
    assert(cert_sn != nullptr);
    char* cert_sn_str = X509_NAME_oneline(cert_sn, 0, 0);
    assert(cert_sn_str != nullptr);

    property.name("dds.cert.sn");
    property.value() = cert_sn_str;
    property.propagate(true);
    token.properties().push_back(std::move(property));

    property.name("dds.cert.algo");
    property.value() = handle->sign_alg_;
    property.propagate(true);
    token.properties().push_back(std::move(property));

    property.name("dds.ca.sn");
    property.value() = handle->sn;
    property.propagate(true);
    token.properties().push_back(std::move(property));

    property.name("dds.ca.algo");
    property.value() = handle->algo;
    property.propagate(true);
    token.properties().push_back(std::move(property));

    CRYPTO_free(cert_sn_str);

    return true;
}

ValidationResult_t PKIDH::validate_local_identity(IdentityHandle** local_identity_handle,
        GUID_t& adjusted_participant_key,
        const uint32_t /*domain_id*/,
        const RTPSParticipantAttributes& participant_attr,
        const GUID_t& candidate_participant_key,
        SecurityException& exception)
{
    assert(local_identity_handle);

    PropertyPolicy auth_properties = PropertyPolicyHelper::get_properties_with_prefix(participant_attr.properties, "dds.sec.auth.builtin.PKI-DH.");

    if(PropertyPolicyHelper::length(auth_properties) == 0)
    {
        exception = _SecurityException_("Not found any dds.sec.auth.builtin.PKI-DH property");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::string* identity_ca = PropertyPolicyHelper::find_property(auth_properties, "identity_ca");

    if(identity_ca == nullptr)
    {
        exception = _SecurityException_("Not found dds.sec.auth.builtin.PKI-DH.identity_ca property");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::string* identity_cert = PropertyPolicyHelper::find_property(auth_properties, "identity_certificate");

    if(identity_cert == nullptr)
    {
        exception = _SecurityException_("Not found dds.sec.auth.builtin.PKI-DH.identity_certificate property");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::string* identity_crl = PropertyPolicyHelper::find_property(auth_properties, "identity_crl");

    std::string* private_key = PropertyPolicyHelper::find_property(auth_properties, "private_key");

    if(private_key == nullptr)
    {
        exception = _SecurityException_("Not found dds.sec.auth.builtin.PKI-DH.private_key property");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::string* password = PropertyPolicyHelper::find_property(auth_properties, "password");
    std::string empty_password;

    if(password == nullptr)
        password = &empty_password;

    PKIIdentityHandle* ih = new PKIIdentityHandle();

    (*ih)->store_ = load_identity_ca(*identity_ca, (*ih)->there_are_crls_, (*ih)->sn, (*ih)->algo,
            exception);

    if((*ih)->store_ != nullptr)
    {
        ERR_clear_error();

        if(identity_crl != nullptr)
        {
            X509_CRL* crl = load_crl(*identity_crl, exception);

            if(crl != nullptr)
            {
                X509_STORE_add_crl((*ih)->store_, crl);
                X509_CRL_free(crl);
                (*ih)->there_are_crls_ = true;
            }
            else
            {
                delete ih;
                return ValidationResult_t::VALIDATION_FAILED;
            }
        }

        ERR_clear_error();

        (*ih)->cert_ = load_certificate(*identity_cert, exception);

        if((*ih)->cert_ != nullptr)
        {
            if(verify_certificate((*ih)->store_, (*ih)->cert_, (*ih)->there_are_crls_))
            {
                if(store_certificate_in_buffer((*ih)->cert_, &(*ih)->cert_content_, exception))
                {
                    if(get_signature_algorithm((*ih)->cert_, (*ih)->sign_alg_, exception))
                    {
                        (*ih)->pkey_ = load_private_key((*ih)->cert_, *private_key, *password, exception);

                        if((*ih)->pkey_ != nullptr)
                        {
                            if(adjust_participant_key((*ih)->cert_, candidate_participant_key, adjusted_participant_key, exception))
                            {
                                // Generate IdentityToken.
                                if(generate_identity_token(*ih))
                                {
                                    (*ih)->participant_key_ = adjusted_participant_key;
                                    *local_identity_handle = ih;

                                    return ValidationResult_t::VALIDATION_OK;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    delete ih;

    ERR_clear_error();

    return ValidationResult_t::VALIDATION_FAILED;
}

ValidationResult_t PKIDH::validate_remote_identity(IdentityHandle** remote_identity_handle,
        const IdentityHandle& local_identity_handle,
        IdentityToken&& remote_identity_token,
        const GUID_t& remote_participant_key,
        SecurityException& /*exception*/)
{
    assert(remote_identity_handle);
    assert(local_identity_handle.nil() == false);

    ValidationResult_t returnedValue = ValidationResult_t::VALIDATION_FAILED;

    const PKIIdentityHandle& lih = PKIIdentityHandle::narrow(local_identity_handle);

    if(!lih.nil())
    {
        // dds.ca.sn
        const std::string* property = DataHolderHelper::find_property_value(remote_identity_token, "dds.ca.sn");

        if(property == nullptr)
        {
            logWarning(SECURITY_AUTHENTICATION, "Not found property \"dds.ca.sn\" in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }

        if(*property != lih->sn)
        {
            logWarning(SECURITY_AUTHENTICATION, "Invalid CA subject name in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }

        // dds.ca.algo
        property = DataHolderHelper::find_property_value(remote_identity_token, "dds.ca.algo");

        if(property == nullptr)
        {
            logWarning(SECURITY_AUTHENTICATION, "Not found property \"dds.ca.algo\" in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }

        if(*property != lih->algo)
        {
            logWarning(SECURITY_AUTHENTICATION, "Invalid CA signature algorithm in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }

        // dds.cert.sn
        const std::string* cert_sn = DataHolderHelper::find_property_value(remote_identity_token, "dds.cert.sn");

        if(cert_sn == nullptr)
        {
            logWarning(SECURITY_AUTHENTICATION, "Not found property \"dds.cert.sn\" in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }

        // dds.cert.algo
        const std::string* cert_algo = DataHolderHelper::find_property_value(remote_identity_token, "dds.cert.algo");

        if(cert_algo == nullptr)
        {
            logWarning(SECURITY_AUTHENTICATION, "Not found property \"dds.cert.algo\" in remote identity token");
            return ValidationResult_t::VALIDATION_FAILED;
        }


        PKIIdentityHandle* rih = new PKIIdentityHandle();

        (*rih)->sn = *cert_sn;
        (*rih)->algo = *cert_algo;
        (*rih)->participant_key_ = remote_participant_key;
        (*rih)->identity_token_ = std::move(remote_identity_token);
        *remote_identity_handle = rih;

        if(lih->participant_key_ < remote_participant_key )
            returnedValue = ValidationResult_t::VALIDATION_PENDING_HANDSHAKE_REQUEST;
        else
            returnedValue = ValidationResult_t::VALIDATION_PENDING_HANDSHAKE_MESSAGE;
    }

    return  returnedValue;
}

ValidationResult_t PKIDH::begin_handshake_request(HandshakeHandle** handshake_handle,
        HandshakeMessageToken** handshake_message,
        const IdentityHandle& initiator_identity_handle,
        IdentityHandle& replier_identity_handle,
        const CDRMessage_t& cdr_participant_data,
        SecurityException& exception)
{
    assert(handshake_handle);
    assert(handshake_message);
    assert(initiator_identity_handle.nil() == false);
    assert(replier_identity_handle.nil() == false);

    const PKIIdentityHandle& lih = PKIIdentityHandle::narrow(initiator_identity_handle);
    PKIIdentityHandle& rih = PKIIdentityHandle::narrow(replier_identity_handle);

    if(lih.nil() || rih.nil())
    {
        exception = _SecurityException_("Bad precondition");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    unsigned char md[SHA256_DIGEST_LENGTH];

    // New handshake
    PKIHandshakeHandle* handshake_handle_aux = new PKIHandshakeHandle();
    (*handshake_handle_aux)->kagree_alg_ = lih->kagree_alg_;
    (*handshake_handle_aux)->handshake_message_.class_id("DDS:Auth:PKI-DH:1.0+Req");

    BinaryProperty bproperty;

    // c.id
    bproperty.name("c.id");
    bproperty.value().assign(lih->cert_content_->data,
            lih->cert_content_->data + lih->cert_content_->length);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // c.pdata
    bproperty.name("c.pdata");
    bproperty.value().assign(cdr_participant_data.buffer,
            cdr_participant_data.buffer + cdr_participant_data.length);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // c.dsign_algo.
    bproperty.name("c.dsign_algo");
    bproperty.value().assign(lih->sign_alg_.begin(),
            lih->sign_alg_.end());
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // TODO(Ricardo) Only support right now DH+MODP-2048-256
    // c.kagree_algo.
    bproperty.name("c.kagree_algo");
    bproperty.value().assign(lih->kagree_alg_.begin(),
            lih->kagree_alg_.end());
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // hash_c1
    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t message(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size((*handshake_handle_aux)->handshake_message_.binary_properties())) + 3);
    message.msg_endian = BIGEND;
    CDRMessage::addBinaryPropertySeq(&message, (*handshake_handle_aux)->handshake_message_.binary_properties());
    if(!EVP_Digest(message.buffer, message.length, md, NULL, EVP_sha256(), NULL))
    {
        exception = _SecurityException_("OpenSSL library cannot hash sha256");
        delete handshake_handle_aux;
        return ValidationResult_t::VALIDATION_FAILED;
    }
    bproperty.name("hash_c1");
    bproperty.value().assign(md, md + SHA256_DIGEST_LENGTH);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // dh1
    if(((*handshake_handle_aux)->dhkeys_ = generate_dh_key(get_dh_type((*handshake_handle_aux)->kagree_alg_), exception)) != nullptr)
    {
        bproperty.name("dh1");
        bproperty.propagate(true);

        if(store_dh_public_key((*handshake_handle_aux)->dhkeys_, bproperty.value(), exception))
        {
            (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

            // challenge1
            bproperty.name("challenge1");
            bproperty.propagate(true);
            if(generate_challenge(bproperty.value(), exception))
            {
                (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

                (*handshake_handle_aux)->local_identity_handle_ = &lih;
                (*handshake_handle_aux)->remote_identity_handle_ = &rih;
                *handshake_handle = handshake_handle_aux;
                *handshake_message = &(*handshake_handle_aux)->handshake_message_;
                return ValidationResult_t::VALIDATION_PENDING_HANDSHAKE_MESSAGE;
            }
        }
    }

    delete handshake_handle_aux;

    ERR_clear_error();

    return ValidationResult_t::VALIDATION_FAILED;
}

ValidationResult_t PKIDH::begin_handshake_reply(HandshakeHandle** handshake_handle,
        HandshakeMessageToken** handshake_message_out,
        HandshakeMessageToken&& handshake_message_in,
        IdentityHandle& initiator_identity_handle,
        const IdentityHandle& replier_identity_handle,
        const CDRMessage_t& cdr_participant_data,
        SecurityException& exception)
{
    assert(handshake_handle);
    assert(handshake_message_out);
    assert(initiator_identity_handle.nil() == false);
    assert(replier_identity_handle.nil() == false);

    const PKIIdentityHandle& lih = PKIIdentityHandle::narrow(replier_identity_handle);
    PKIIdentityHandle& rih = PKIIdentityHandle::narrow(initiator_identity_handle);

    if(lih.nil() || rih.nil())
    {
        exception = _SecurityException_("Bad precondition");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check TokenMessage
    if(handshake_message_in.class_id().compare("DDS:Auth:PKI-DH:1.0+Req") != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad HandshakeMessageToken (" << handshake_message_in.class_id() << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check incomming handshake.
    // Check c.id
    const std::vector<uint8_t>* cid = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.id");
    if(cid == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.id");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    rih->cert_ = load_certificate(*cid);

    if(rih->cert_ == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot load certificate");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    X509_NAME* cert_sn = X509_get_subject_name(rih->cert_);
    assert(cert_sn != nullptr);
    char* cert_sn_str = X509_NAME_oneline(cert_sn, 0, 0);
    assert(cert_sn_str != nullptr);
    if(rih->sn.compare(cert_sn_str) != 0)
    {
        CRYPTO_free(cert_sn_str);
        logWarning(SECURITY_AUTHENTICATION, "Certificated subject name invalid");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    CRYPTO_free(cert_sn_str);

    if(!verify_certificate(lih->store_, rih->cert_, lih->there_are_crls_))
    {
        logWarning(SECURITY_AUTHENTICATION, "Error verifying certificate");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* pdata = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.pdata");

    if(pdata == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t cdr_pdata(0);
    cdr_pdata.wraps = true;
    cdr_pdata.msg_endian = BIGEND;
    cdr_pdata.length = (uint32_t)pdata->size();
    cdr_pdata.max_size = (uint32_t)pdata->size();
    cdr_pdata.buffer = (octet*)pdata->data();
    ParticipantProxyData remote_participant_data;
    if(!remote_participant_data.readFromCDRMessage(&cdr_pdata))
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot deserialize ParticipantProxyData in property c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if((remote_participant_data.m_guid.guidPrefix.value[0] & 0x80) != 0x80)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad participant_key's first bit in c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned int length = 0;

    if(!X509_NAME_digest(cert_sn, EVP_sha256(), md, &length) || length != SHA256_DIGEST_LENGTH)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot generate SHA256 of subject name");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    md[5] &= 0xFE;
    unsigned char bytes[6]{
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[0] << 1) | (remote_participant_data.m_guid.guidPrefix.value[1] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[1] << 1) | (remote_participant_data.m_guid.guidPrefix.value[2] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[2] << 1) | (remote_participant_data.m_guid.guidPrefix.value[3] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[3] << 1) | (remote_participant_data.m_guid.guidPrefix.value[4] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[4] << 1) | (remote_participant_data.m_guid.guidPrefix.value[5] >> 7)),
        static_cast<unsigned char>(remote_participant_data.m_guid.guidPrefix.value[5] << 1)
    };

    if(memcmp(md, bytes, 6) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad participant_key's 47bits in c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // c.dsign_algo
    const std::vector<uint8_t>* dsign_algo = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.dsign_algo");

    if(dsign_algo == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.dsign_algo");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check signature algorithm
    std::string s_dsign_algo(dsign_algo->begin(), dsign_algo->end());
    if(s_dsign_algo.compare(RSA_SHA256) != 0 &&
            s_dsign_algo.compare(ECDSA_SHA256) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Not supported signature algorithm (" << s_dsign_algo << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    rih->sign_alg_ = std::move(s_dsign_algo);

    // c.kagree_algo
    const std::vector<uint8_t>* kagree_algo = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.kagree_algo");

    if(kagree_algo == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.kagree_algo");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check key agreement algorithm
    std::string s_kagree_algo(kagree_algo->begin(), kagree_algo->end());
    if(s_kagree_algo.compare(DH_2048_256) != 0 &&
            s_kagree_algo.compare(ECDH_prime256v1) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Not supported key agreement algorithm (" << s_kagree_algo << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    rih->kagree_alg_ = std::move(s_kagree_algo);

    // hash_c1
    std::vector<uint8_t>* hash_c1 = DataHolderHelper::find_binary_property_value(handshake_message_in, "hash_c1");

    if(hash_c1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(hash_c1->size() != SHA256_DIGEST_LENGTH)
    {
        logWarning(SECURITY_AUTHENTICATION, "Wrong size of hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t cdrmessage(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size(handshake_message_in.binary_properties())) + 3);
    cdrmessage.msg_endian = BIGEND;
    CDRMessage::addBinaryPropertySeq(&cdrmessage, handshake_message_in.binary_properties(), "hash_c1");

    if(!EVP_Digest(cdrmessage.buffer, cdrmessage.length, md, NULL, EVP_sha256(), NULL))
    {
        exception = _SecurityException_("Cannot generate SHA256 of request");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    
    if(memcmp(md, hash_c1->data(), SHA256_DIGEST_LENGTH) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Wrong hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // dh1
    std::vector<uint8_t>* dh1 = DataHolderHelper::find_binary_property_value(handshake_message_in, "dh1");

    if(dh1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // challenge1
    std::vector<uint8_t>* challenge1 = DataHolderHelper::find_binary_property_value(handshake_message_in, "challenge1");

    if(challenge1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property challenge1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Generate handshake reply message token.
    PKIHandshakeHandle* handshake_handle_aux = new PKIHandshakeHandle();
    (*handshake_handle_aux)->kagree_alg_ = rih->kagree_alg_;
    (*handshake_handle_aux)->handshake_message_.class_id("DDS:Auth:PKI-DH:1.0+Reply");

    // Store dh1
    if(((*handshake_handle_aux)->peerkeys_ = generate_dh_peer_key(*dh1, exception)) == nullptr)
    {
        exception = _SecurityException_("Cannot store peer key from dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    BinaryProperty bproperty;

    // c.id
    bproperty.name("c.id");
    bproperty.value().assign(lih->cert_content_->data,
            lih->cert_content_->data + lih->cert_content_->length);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // c.pdata
    bproperty.name("c.pdata");
    bproperty.value().assign(cdr_participant_data.buffer,
            cdr_participant_data.buffer + cdr_participant_data.length);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // c.dsign_algo.
    bproperty.name("c.dsign_algo");
    bproperty.value().assign(lih->sign_alg_.begin(),
            lih->sign_alg_.end());
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // TODO(Ricardo) Only support right now DH+MODP-2048-256
    // c.kagree_algo.
    bproperty.name("c.kagree_algo");
    bproperty.value().assign((*handshake_handle_aux)->kagree_alg_.begin(),
            (*handshake_handle_aux)->kagree_alg_.end());
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

    // hash_c2
    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t message(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size((*handshake_handle_aux)->handshake_message_.binary_properties())) + 3);
    message.msg_endian = BIGEND;
    CDRMessage::addBinaryPropertySeq(&message, (*handshake_handle_aux)->handshake_message_.binary_properties());
    if(!EVP_Digest(message.buffer, message.length, md, NULL, EVP_sha256(), NULL))
    {
        exception = _SecurityException_("OpenSSL library cannot hash sha256");
        delete handshake_handle_aux;
        return ValidationResult_t::VALIDATION_FAILED;
    }
    bproperty.name("hash_c2");
    bproperty.value().assign(md, md + SHA256_DIGEST_LENGTH);
    bproperty.propagate(true);
    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));
    

    // dh2
    if(((*handshake_handle_aux)->dhkeys_ = generate_dh_key(get_dh_type((*handshake_handle_aux)->kagree_alg_), exception)) != nullptr)
    {
        bproperty.name("dh2");
        bproperty.propagate(true);

        if(store_dh_public_key((*handshake_handle_aux)->dhkeys_, bproperty.value(), exception))
        {
            (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

            // hash_c1
            bproperty.name("hash_c1");
            bproperty.value(std::move(*hash_c1));
            bproperty.propagate(true);
            (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

            // dh1
            bproperty.name("dh1");
            bproperty.value(std::move(*dh1));
            bproperty.propagate(true);
            (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

            // challenge1
            bproperty.name("challenge1");
            bproperty.value(std::move(*challenge1));
            bproperty.propagate(true);
            (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

            // challenge2
            bproperty.name("challenge2");
            bproperty.propagate(true);
            if(generate_challenge(bproperty.value(), exception))
            {
                (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

                // signature
                CDRMessage_t cdrmessage2(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size((*handshake_handle_aux)->handshake_message_.binary_properties())));
                cdrmessage2.msg_endian = BIGEND;
                // add sequence length
                CDRMessage::addUInt32(&cdrmessage2, 6);
                //add hash_c2
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "hash_c2"));
                //add challenge2
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "challenge2"));
                //add dh2
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "dh2"));
                //add challenge1
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "challenge1"));
                //add dh1
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "dh1"));
                //add hash_c1
                CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property((*handshake_handle_aux)->handshake_message_, "hash_c1"));

                bproperty.name("signature");
                bproperty.propagate("true");
                if(sign_sha256(lih->pkey_, cdrmessage2.buffer, cdrmessage2.length, bproperty.value(), exception))
                {
                    (*handshake_handle_aux)->handshake_message_.binary_properties().push_back(std::move(bproperty));

                    (*handshake_handle_aux)->local_identity_handle_ = &lih;
                    (*handshake_handle_aux)->remote_identity_handle_ = &rih;
                    *handshake_handle = handshake_handle_aux;
                    *handshake_message_out = &(*handshake_handle_aux)->handshake_message_;

                    return ValidationResult_t::VALIDATION_PENDING_HANDSHAKE_MESSAGE;
                }
            }
        }
    }

    delete handshake_handle_aux;

    ERR_clear_error();

    return ValidationResult_t::VALIDATION_FAILED;
}

ValidationResult_t PKIDH::process_handshake(HandshakeMessageToken** handshake_message_out,
        HandshakeMessageToken&& handshake_message_in,
        HandshakeHandle& handshake_handle,
        SecurityException& exception)
{
    ValidationResult_t returnedValue = ValidationResult_t::VALIDATION_FAILED;

    PKIHandshakeHandle& handshake = PKIHandshakeHandle::narrow(handshake_handle);

    if(!handshake.nil())
    {
        if(handshake->handshake_message_.class_id().compare("DDS:Auth:PKI-DH:1.0+Req") == 0)
        {
            returnedValue = process_handshake_request(handshake_message_out, std::move(handshake_message_in),
                    handshake, exception);
        }
        else if(handshake->handshake_message_.class_id().compare("DDS:Auth:PKI-DH:1.0+Reply") == 0)
        {
            returnedValue = process_handshake_reply(handshake_message_out, std::move(handshake_message_in),
                    handshake, exception);
        } 
        else
        {
            logWarning(SECURITY_AUTHENTICATION, "Handshake message not supported (" << handshake->handshake_message_.class_id() << ")");
        }
    }

    return returnedValue;
}

ValidationResult_t PKIDH::process_handshake_request(HandshakeMessageToken** handshake_message_out,
        HandshakeMessageToken&& handshake_message_in,
        PKIHandshakeHandle& handshake_handle,
        SecurityException& exception)
{
    const PKIIdentityHandle& lih = *handshake_handle->local_identity_handle_;
    PKIIdentityHandle& rih = *handshake_handle->remote_identity_handle_;

    // Check TokenMessage
    if(handshake_message_in.class_id().compare("DDS:Auth:PKI-DH:1.0+Reply") != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad HandshakeMessageToken (" << handshake_message_in.class_id() << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check incomming handshake.
    // Check c.id
    const std::vector<uint8_t>* cid = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.id");
    if(cid == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.id");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    rih->cert_ = load_certificate(*cid);

    if(rih->cert_ == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot load certificate");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(!verify_certificate(lih->store_, rih->cert_, lih->there_are_crls_))
    {
        logWarning(SECURITY_AUTHENTICATION, "Error verifying certificate");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* pdata = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.pdata");

    if(pdata == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t cdr_pdata(0);
    cdr_pdata.wraps = true;
    cdr_pdata.msg_endian = BIGEND;
    cdr_pdata.length = (uint32_t)pdata->size();
    cdr_pdata.max_size = (uint32_t)pdata->size();
    cdr_pdata.buffer = (octet*)pdata->data();
    ParticipantProxyData remote_participant_data;
    if(!remote_participant_data.readFromCDRMessage(&cdr_pdata))
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot deserialize ParticipantProxyData in property c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if((remote_participant_data.m_guid.guidPrefix.value[0] & 0x80) != 0x80)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad participant_key's first bit in c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    X509_NAME* cert_sn = X509_get_subject_name(rih->cert_);
    assert(cert_sn != nullptr);

    unsigned char md[SHA256_DIGEST_LENGTH];
    unsigned int length = 0;

    if(!X509_NAME_digest(cert_sn, EVP_sha256(), md, &length) || length != SHA256_DIGEST_LENGTH)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot generate SHA256 of subject name");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    md[5] &= 0xFE;
    unsigned char bytes[6]{
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[0] << 1) | (remote_participant_data.m_guid.guidPrefix.value[1] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[1] << 1) | (remote_participant_data.m_guid.guidPrefix.value[2] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[2] << 1) | (remote_participant_data.m_guid.guidPrefix.value[3] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[3] << 1) | (remote_participant_data.m_guid.guidPrefix.value[4] >> 7)),
        static_cast<unsigned char>((remote_participant_data.m_guid.guidPrefix.value[4] << 1) | (remote_participant_data.m_guid.guidPrefix.value[5] >> 7)),
        static_cast<unsigned char>(remote_participant_data.m_guid.guidPrefix.value[5] << 1)
    };

    if(memcmp(md, bytes, 6) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad participant_key's 47bits in c.pdata");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    
    // c.dsign_algo
    const std::vector<uint8_t>* dsign_algo = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.dsign_algo");

    if(dsign_algo == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.dsign_algo");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check signature algorithm
    std::string s_dsign_algo(dsign_algo->begin(), dsign_algo->end());
    if(s_dsign_algo.compare(RSA_SHA256) != 0 &&
            s_dsign_algo.compare(ECDSA_SHA256) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Not supported signature algorithm (" << s_dsign_algo << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    rih->sign_alg_ = std::move(s_dsign_algo);

    // c.kagree_algo
    const std::vector<uint8_t>* kagree_algo = DataHolderHelper::find_binary_property_value(handshake_message_in, "c.kagree_algo");

    if(kagree_algo == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property c.kagree_algo");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check key agreement algorithm
    std::string s_kagree_algo(kagree_algo->begin(), kagree_algo->end());
    if(s_kagree_algo.compare(handshake_handle->kagree_alg_) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid key agreement algorithm. Received " << s_kagree_algo << ", expected " << handshake_handle->kagree_alg_);
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // hash_c2
    BinaryProperty* hash_c2 = DataHolderHelper::find_binary_property(handshake_message_in, "hash_c2");

    if(hash_c2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property hash_c2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(hash_c2->value().size() != SHA256_DIGEST_LENGTH)
    {
        logWarning(SECURITY_AUTHENTICATION, "Wrong size of hash_c2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // TODO(Ricardo) Have to add +3 because current serialization add alignment bytes at the end.
    CDRMessage_t cdrmessage(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size(handshake_message_in.binary_properties()))+ 3);
    cdrmessage.msg_endian = BIGEND;
    CDRMessage::addBinaryPropertySeq(&cdrmessage, handshake_message_in.binary_properties(), "hash_c2");

    if(!EVP_Digest(cdrmessage.buffer, cdrmessage.length, md, NULL, EVP_sha256(), NULL))
    {
        exception = _SecurityException_("Cannot generate SHA256 of request");
        return ValidationResult_t::VALIDATION_FAILED;
    }
    
    if(memcmp(md, hash_c2->value().data(), SHA256_DIGEST_LENGTH) != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Wrong hash_c2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // dh2
    BinaryProperty* dh2 = DataHolderHelper::find_binary_property(handshake_message_in, "dh2");

    if(dh2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property dh2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if((handshake_handle->peerkeys_ = generate_dh_peer_key(dh2->value(), exception)) == nullptr)
    {
        exception = _SecurityException_("Cannot store peer key from dh2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    BinaryProperty* challenge2 = DataHolderHelper::find_binary_property(handshake_message_in, "challenge2");

    if(challenge2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property challenge2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // hash_c1
    BinaryProperty* hash_c1 = DataHolderHelper::find_binary_property(handshake_message_in, "hash_c1");

    if(hash_c1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* hash_c1_request = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "hash_c1");

    if(hash_c1_request == nullptr)
    {
        exception = _SecurityException_("Cannot find property hash_c1 in request message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(hash_c1->value() != *hash_c1_request)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid property hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // dh1
    BinaryProperty* dh1 = DataHolderHelper::find_binary_property(handshake_message_in, "dh1");

    if(dh1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* dh1_request = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "dh1");

    if(dh1_request == nullptr)
    {
        exception = _SecurityException_("Cannot find property dh1 in request message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(dh1->value() != *dh1_request)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid property dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    BinaryProperty* challenge1 = DataHolderHelper::find_binary_property(handshake_message_in, "challenge1");

    if(challenge1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property challenge1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* challenge1_request = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "challenge1");

    if(challenge1_request == nullptr)
    {
        exception = _SecurityException_("Cannot find property challenge1 in request message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(challenge1->value() != *challenge1_request)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid property challenge1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* signature = DataHolderHelper::find_binary_property_value(handshake_message_in, "signature");

    if(signature == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property signature");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // signature
    CDRMessage_t cdrmessage2(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size(handshake_message_in.binary_properties())));
    cdrmessage2.msg_endian = BIGEND;
    // add sequence length
    CDRMessage::addUInt32(&cdrmessage2, 6);
    //add hash_c2
    CDRMessage::addBinaryProperty(&cdrmessage2, *hash_c2);
    //add challenge2
    CDRMessage::addBinaryProperty(&cdrmessage2, *challenge2);
    //add dh2
    CDRMessage::addBinaryProperty(&cdrmessage2, *dh2);
    //add challenge1
    CDRMessage::addBinaryProperty(&cdrmessage2, *challenge1);
    //add dh1
    CDRMessage::addBinaryProperty(&cdrmessage2, *dh1);
    //add hash_c1
    CDRMessage::addBinaryProperty(&cdrmessage2, *hash_c1);

    if(!check_sign_sha256(rih->cert_, cdrmessage2.buffer, cdrmessage2.length, *signature, exception))
    {
        logWarning(SECURITY_AUTHENTICATION, "Error verifying signature");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Generate handshake final message token.
    HandshakeMessageToken final_message;
    final_message.binary_properties().clear();
    final_message.class_id("DDS:Auth:PKI-DH:1.0+Final");

    BinaryProperty bproperty;

    // hash_c1
    bproperty.name("hash_c1");
    bproperty.value(std::move(hash_c1->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // hash_c2
    bproperty.name("hash_c2");
    bproperty.value(std::move(hash_c2->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // dh1
    bproperty.name("dh1");
    bproperty.value(std::move(dh1->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // dh2
    bproperty.name("dh2");
    bproperty.value(std::move(dh2->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // challenge1
    bproperty.name("challenge1");
    bproperty.value(std::move(challenge1->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // challenge2
    bproperty.name("challenge2");
    bproperty.value(std::move(challenge2->value()));
    bproperty.propagate(true);
    final_message.binary_properties().push_back(std::move(bproperty));

    // signature
    cdrmessage2.length = 0;
    cdrmessage2.pos = 0;
    // add sequence length
    CDRMessage::addUInt32(&cdrmessage2, 6);
    //add hash_c1
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "hash_c1"));
    //add challenge1
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "challenge1"));
    //add dh1
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "dh1"));
    //add challenge2
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "challenge2"));
    //add dh2
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "dh2"));
    //add hash_c2
    CDRMessage::addBinaryProperty(&cdrmessage2, *DataHolderHelper::find_binary_property(final_message, "hash_c2"));

    bproperty.name("signature");
    bproperty.propagate("true");
    if(sign_sha256(lih->pkey_, cdrmessage2.buffer, cdrmessage2.length, bproperty.value(), exception))
    {
        final_message.binary_properties().push_back(std::move(bproperty));

        handshake_handle->sharedsecret_ = generate_sharedsecret(handshake_handle->dhkeys_, handshake_handle->peerkeys_,
                exception);

        if(handshake_handle->sharedsecret_ != nullptr)
        {
            // Save challenge1 y challenge2 in sharedsecret
            (*handshake_handle->sharedsecret_)->data_.emplace_back(SharedSecret::BinaryData("Challenge1",
                        *DataHolderHelper::find_binary_property_value(final_message, "challenge1")));
            (*handshake_handle->sharedsecret_)->data_.emplace_back(SharedSecret::BinaryData("Challenge2",
                        *DataHolderHelper::find_binary_property_value(final_message, "challenge2")));

            handshake_handle->handshake_message_ = std::move(final_message);
            *handshake_message_out = &handshake_handle->handshake_message_;

            return ValidationResult_t::VALIDATION_OK_WITH_FINAL_MESSAGE;
        }
    }

    ERR_clear_error();

    return ValidationResult_t::VALIDATION_FAILED;
}

ValidationResult_t PKIDH::process_handshake_reply(HandshakeMessageToken** /*handshake_message_out*/,
        HandshakeMessageToken&& handshake_message_in,
        PKIHandshakeHandle& handshake_handle,
        SecurityException& exception)
{
    PKIIdentityHandle& rih = *handshake_handle->remote_identity_handle_;

    // Check TokenMessage
    if(handshake_message_in.class_id().compare("DDS:Auth:PKI-DH:1.0+Final") != 0)
    {
        logWarning(SECURITY_AUTHENTICATION, "Bad HandshakeMessageToken (" << handshake_message_in.class_id() << ")");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // Check incomming handshake.
    // hash_c1
    BinaryProperty* hash_c1 = DataHolderHelper::find_binary_property(handshake_message_in, "hash_c1");

    if(hash_c1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* hash_c1_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "hash_c1");

    if(hash_c1_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property hash_c1 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(hash_c1->value() != *hash_c1_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid hash_c1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // hash_c2
    BinaryProperty* hash_c2 = DataHolderHelper::find_binary_property(handshake_message_in, "hash_c2");

    if(hash_c2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property hash_c2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* hash_c2_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "hash_c2");

    if(hash_c2_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property hash_c2 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(hash_c2->value() != *hash_c2_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid hash_c2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // dh1
    BinaryProperty* dh1 = DataHolderHelper::find_binary_property(handshake_message_in, "dh1");

    if(dh1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* dh1_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "dh1");

    if(dh1_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property dh1 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(dh1->value() != *dh1_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid dh1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // dh2
    BinaryProperty* dh2 = DataHolderHelper::find_binary_property(handshake_message_in, "dh2");

    if(dh2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property dh2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* dh2_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "dh2");

    if(dh2_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property dh2 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(dh2->value() != *dh2_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid dh2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    BinaryProperty* challenge1 = DataHolderHelper::find_binary_property(handshake_message_in, "challenge1");

    if(challenge1 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property challenge1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* challenge1_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "challenge1");

    if(challenge1_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property challenge1 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(challenge1->value() != *challenge1_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid challenge1");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    BinaryProperty* challenge2 = DataHolderHelper::find_binary_property(handshake_message_in, "challenge2");

    if(challenge2 == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property challenge2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    std::vector<uint8_t>* challenge2_reply = DataHolderHelper::find_binary_property_value(handshake_handle->handshake_message_, "challenge2");

    if(challenge2_reply == nullptr)
    {
        exception = _SecurityException_("Cannot find property challenge2 in reply message");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    if(challenge2->value() != *challenge2_reply)
    {
        logWarning(SECURITY_AUTHENTICATION, "Invalid challenge2");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    const std::vector<uint8_t>* signature = DataHolderHelper::find_binary_property_value(handshake_message_in, "signature");

    if(signature == nullptr)
    {
        logWarning(SECURITY_AUTHENTICATION, "Cannot find property signature");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    // signature
    CDRMessage_t cdrmessage(static_cast<uint32_t>(BinaryPropertyHelper::serialized_size(handshake_message_in.binary_properties())));
    cdrmessage.msg_endian = BIGEND;
    // add sequence length
    CDRMessage::addUInt32(&cdrmessage, 6);
    //add hash_c1
    CDRMessage::addBinaryProperty(&cdrmessage, *hash_c1);
    //add challenge1
    CDRMessage::addBinaryProperty(&cdrmessage, *challenge1);
    //add dh1
    CDRMessage::addBinaryProperty(&cdrmessage, *dh1);
    //add challenge2
    CDRMessage::addBinaryProperty(&cdrmessage, *challenge2);
    //add dh2
    CDRMessage::addBinaryProperty(&cdrmessage, *dh2);
    //add hash_c2
    CDRMessage::addBinaryProperty(&cdrmessage, *hash_c2);

    if(!check_sign_sha256(rih->cert_, cdrmessage.buffer, cdrmessage.length, *signature, exception))
    {
        logWarning(SECURITY_AUTHENTICATION, "Error verifying signature");
        return ValidationResult_t::VALIDATION_FAILED;
    }

    handshake_handle->sharedsecret_ = generate_sharedsecret(handshake_handle->dhkeys_, handshake_handle->peerkeys_,
            exception);

    if(handshake_handle->sharedsecret_ != nullptr)
    {
        // Save challenge1 y challenge2 in sharedsecret
        (*handshake_handle->sharedsecret_)->data_.emplace_back(SharedSecret::BinaryData("Challenge1",
                    challenge1->value()));
        (*handshake_handle->sharedsecret_)->data_.emplace_back(SharedSecret::BinaryData("Challenge2",
                    challenge2->value()));

        return ValidationResult_t::VALIDATION_OK;
    }

    ERR_clear_error();

    return ValidationResult_t::VALIDATION_FAILED;
}

SharedSecretHandle* PKIDH::get_shared_secret(const HandshakeHandle& handshake_handle,
        SecurityException& /*exception*/)
{
    const PKIHandshakeHandle& handshake = PKIHandshakeHandle::narrow(handshake_handle);

    if(!handshake.nil())
    {
        SharedSecretHandle* sharedsecret = new SharedSecretHandle();
        (*sharedsecret)->data_ = (*handshake->sharedsecret_)->data_;
        return sharedsecret;
    }

    return nullptr;
}

bool PKIDH::set_listener(AuthenticationListener* /*listener*/,
        SecurityException& /*exception*/)
{
    return false;
}

bool PKIDH::get_identity_token(IdentityToken** identity_token,
        const IdentityHandle& handle,
        SecurityException& /*exception*/)
{
    const PKIIdentityHandle& ihandle = PKIIdentityHandle::narrow(handle);

    if(!ihandle.nil())
    {
        *identity_token = new IdentityToken(ihandle->identity_token_);
        return true;
    }

    return false;
}

bool PKIDH::return_identity_token(IdentityToken* token,
        SecurityException& /*exception*/)
{
    delete token;
    return true;
}

bool PKIDH::return_handshake_handle(HandshakeHandle* handshake_handle,
        SecurityException& /*exception*/)
{
    PKIHandshakeHandle* handle = &PKIHandshakeHandle::narrow(*handshake_handle);

    if(!handle->nil())
    {
        delete handle;
        return true;
    }

    return false;
}

bool PKIDH::return_identity_handle(IdentityHandle* identity_handle,
        SecurityException& /*exception*/)
{
    PKIIdentityHandle* handle = &PKIIdentityHandle::narrow(*identity_handle);

    if(!handle->nil())
    {
        delete handle;
        return true;
    }

    return false;
}

bool PKIDH::return_sharedsecret_handle(SharedSecretHandle* sharedsecret_handle,
        SecurityException& /*exception*/)
{
    delete sharedsecret_handle;
    return true;
}
