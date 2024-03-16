# BASS_CROSSFADE
A fading playback plugin for BASS with .NET bindings.

bass.dll and bassmix.dll are required for native projects.
ManagedBass and ManagedBass.Mix are required for .NET projects.

A simple example;

```c#
BassCrossfade.Load();
BassCrossfade.Mode = BassCrossfadeMode.Always;
BassCrossfade.InPeriod = 1000;
BassCrossfade.OutPeriod = 1000;
BassCrossfade.InType = BassCrossfadeType.Linear;
BassCrossfade.OutType = BassCrossfadeType.Linear;
BassCrossfade.Mix = true;

Bass.Init();

var sourceChannel1 = Bass.CreateStream("01 Botanical Dimensions.m4a", Flags: BassFlags.Decode);
var sourceChannel2 = Bass.CreateStream("02 Outer Shpongolia.m4a", Flags: BassFlags.Decode);

BassCrossfade.ChannelEnqueue(sourceChannel1);
BassCrossfade.ChannelEnqueue(sourceChannel2);

var channelInfo = Bass.ChannelGetInfo(sourceChannel1);
var playbackChannel = BassCrossfade.StreamCreate(channelInfo.Frequency, channelInfo.Channels);

Bass.ChannelPlay(playbackChannel);

while (Bass.ChannelIsActive(sourceChannel1) == PlaybackState.Playing || Bass.ChannelIsActive(sourceChannel2) == PlaybackState.Playing)
{
    Thread.Sleep(1000);
}

Bass.StreamFree(sourceChannel1);
Bass.StreamFree(sourceChannel2);
Bass.StreamFree(playbackChannel);

BassCrossfade.Unload();
Bass.Free();
```

Since version 0.1.0, you are responsible for handling playback latency.
This likely only means adding a delay when performing a manual fade out, e.g; 

```c#
BassCrossfade.StreamFadeOut();
Thread.Sleep(Bass.PlaybackBufferLength);
```

If you have mixers or other buffers, they need to be accounted for. 
Obviously if you want the effect to be responsive, keep the latency to a minimum.