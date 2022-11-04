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

/*************************************************************************
 * @file OwnershipStrengthSubscriber.h
 * This header file contains the declaration of the subscriber functions.
 *
 * This file was generated by the tool fastcdrgen.
 */


#ifndef _OWNERSHIPTEST_SUBSCRIBER_H_
#define _OWNERSHIPTEST_SUBSCRIBER_H_

#include <fastrtps/fastrtps_fwd.h>
#include <fastrtps/subscriber/SubscriberListener.h>
#include <fastrtps/subscriber/SampleInfo.h>
#include "OwnershipStrengthPubSubTypes.h"
#include <set>
#include <mutex>
#include <map>

using namespace eprosima::fastrtps;

class OwnershipStrengthSubscriber 
{
public:
	OwnershipStrengthSubscriber();
	virtual ~OwnershipStrengthSubscriber();
	bool init();
	void run();
private:
	Participant *mp_participant;
	Subscriber *mp_subscriber;

   class StrengthHierarchy
   {
   public:
      // Automatically adds a publisher to the hierarchy, and reports back whether it is the strongest.
      bool IsMessageStrong(const ExampleMessage& st, const SampleInfo_t& info);

      // Deregisters a publisher removing it from the hierarchy.
      void DeregisterPublisher(GUID_t guid);

      // Provides an ordered hierarchy of publishers, based on the Ownership Strength QoS parameter.
      std::map<unsigned int, std::set<GUID_t> > strengthMap; // Keyed by Ownership Strength
      std::mutex mapMutex;
   };

	class SubListener : public SubscriberListener
	{
	public:
		SubListener() : n_matched(0),n_msg(0){};
		~SubListener(){};
		void onSubscriptionMatched(Subscriber* sub,MatchingInfo& info);
		void onNewDataMessage(Subscriber* sub);
		SampleInfo_t m_info;
		int n_matched;
		int n_msg;
      StrengthHierarchy m_hierarchy;
	} m_listener;

	ExampleMessagePubSubType myType;
};

#endif // _OwnershipStrength_SUBSCRIBER_H_
