/******************************************************************************
 * Copyright 2021 The ZhiTo Authors. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include "cyber/examples/proto/examples.pb.h"
#include "cyber/time/time.h"
#include "cyber/cyber.h"
#include "statistic_report/statistic.hpp"

using apollo::cyber::Time;

#define PACKAGE_LEN_10KB 10240
#define PACKAGE_LEN_20KB 20480
#define PACKAGE_LEN_30KB 30720
#define PACKAGE_LEN_100KB 102400

static uint64_t seq = 0;
bool enable_counted = true;

int main(int argc, char* argv[]) {
	apollo::cyber::Init(argv[0]);
	apollo::cyber::ReaderConfig reader_config;
	reader_config.channel_name = "channel/chatter_01";
	reader_config.pending_queue_size = 128;
	
	auto listener_node = apollo::cyber::CreateNode("listener_01");
	listener_node->CreateReader<apollo::cyber::examples::proto::Chatter>(reader_config,
		[&](const std::shared_ptr<const apollo::cyber::examples::proto::Chatter>& msg) 
		{
			std::cout<<"receive package from remote at "<<msg->seq()<<std::endl;
		});

	apollo::cyber::WaitForShutdown();

	return 0;
}
