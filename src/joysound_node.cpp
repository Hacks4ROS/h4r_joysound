/*
 * joySound
 * Copyright (c) 2012, rad-lab.net
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <ORGANIZATION> nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <string>
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "sound_play/SoundRequest.h"

class JoySound
{
public:
	int play_button;
	bool play_, lplay_;
	std::string play_file;

	ros::NodeHandle n;
	ros::Subscriber joy_sub;
	ros::Publisher sound;
	sound_play::SoundRequest sound_req;

	JoySound(ros::NodeHandle &n) :
			n(n)
	{
	}

	void init()
	{

		n.param("play_button", play_button, 0);
		n.param<std::string>("play_file", play_file, "");

		sound = n.advertise<sound_play::SoundRequest>("/robotsound", 1, 0);

		ROS_INFO("play_button: %d", play_button);

		ROS_INFO("play_file: %s", play_file.c_str());

		if (!play_file.size())
			ROS_WARN("SOUND FILE HAS NOT BEEN SET! USE PARAM \"playfile\" FOR SETTING IT!");

		joy_sub = n.subscribe("/joy", 10, &JoySound::joy_cb, this);

		sound_req.sound = sound_play::SoundRequest::PLAY_FILE;
		sound_req.command = sound_play::SoundRequest::PLAY_ONCE;
		sound_req.arg = play_file;
	}

	~JoySound()
	{
	}

	void joy_cb(const sensor_msgs::Joy::ConstPtr& joy_msg)
	{

		play_ = (((unsigned int) play_button < joy_msg->buttons.size()) && joy_msg->buttons[play_button]);

		//Only execute the sound once on each button press
		if (play_)
		{

			if (lplay_ == false)
			{
				lplay_ = true;
				sound.publish(sound_req);
			}
		}
		else
		{
			lplay_ = false;
		}
	}

};

int main(int argc, char **argv)
{
	ros::init(argc, argv, "joySound");
	ros::NodeHandle nh("~");
	JoySound sound(nh);
	sound.init();
	ros::spin();

	exit(0);
	return 0;
}
