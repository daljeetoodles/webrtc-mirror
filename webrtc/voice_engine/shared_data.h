/*
 *  Copyright (c) 2012 The WebRTC project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef WEBRTC_VOICE_ENGINE_SHARED_DATA_H
#define WEBRTC_VOICE_ENGINE_SHARED_DATA_H

#include <memory>

#include "webrtc/modules/audio_device/include/audio_device.h"
#include "webrtc/modules/audio_processing/include/audio_processing.h"
#include "webrtc/modules/utility/include/process_thread.h"
#include "webrtc/rtc_base/criticalsection.h"
#include "webrtc/rtc_base/scoped_ref_ptr.h"
#include "webrtc/rtc_base/task_queue.h"
#include "webrtc/rtc_base/thread_annotations.h"
#include "webrtc/rtc_base/thread_checker.h"
#include "webrtc/voice_engine/channel_manager.h"
#include "webrtc/voice_engine/statistics.h"
#include "webrtc/voice_engine/voice_engine_defines.h"

class ProcessThread;

namespace webrtc {
namespace voe {

class TransmitMixer;
class OutputMixer;

class SharedData
{
public:
    // Public accessors.
    uint32_t instance_id() const { return _instanceId; }
    Statistics& statistics() { return _engineStatistics; }
    ChannelManager& channel_manager() { return _channelManager; }
    AudioDeviceModule* audio_device() { return _audioDevicePtr.get(); }
    void set_audio_device(
        const rtc::scoped_refptr<AudioDeviceModule>& audio_device);
    void set_audio_processing(AudioProcessing* audio_processing);
    TransmitMixer* transmit_mixer() { return _transmitMixerPtr; }
    OutputMixer* output_mixer() { return _outputMixerPtr; }
    rtc::CriticalSection* crit_sec() { return &_apiCritPtr; }
    ProcessThread* process_thread() { return _moduleProcessThreadPtr.get(); }
    rtc::TaskQueue* encoder_queue();

    int NumOfSendingChannels();
    int NumOfPlayingChannels();

    // Convenience methods for calling statistics().SetLastError().
    void SetLastError(int32_t error) const;
    void SetLastError(int32_t error, TraceLevel level) const;
    void SetLastError(int32_t error, TraceLevel level,
                      const char* msg) const;

protected:
 rtc::ThreadChecker construction_thread_;
 const uint32_t _instanceId;
 rtc::CriticalSection _apiCritPtr;
 ChannelManager _channelManager;
 Statistics _engineStatistics;
 rtc::scoped_refptr<AudioDeviceModule> _audioDevicePtr;
 OutputMixer* _outputMixerPtr;
 TransmitMixer* _transmitMixerPtr;
 std::unique_ptr<ProcessThread> _moduleProcessThreadPtr;
 // |encoder_queue| is defined last to ensure all pending tasks are cancelled
 // and deleted before any other members.
 rtc::TaskQueue encoder_queue_ RTC_ACCESS_ON(construction_thread_);

 SharedData();
 virtual ~SharedData();
};

}  // namespace voe
}  // namespace webrtc
#endif // WEBRTC_VOICE_ENGINE_SHARED_DATA_H
