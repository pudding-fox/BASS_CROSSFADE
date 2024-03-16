using System;
using System.IO;
using System.Runtime.InteropServices;

namespace ManagedBass.Crossfade
{
    public class BassCrossfade
    {
        public const int MAX_CHANNELS = 10;

        const BassCrossfadeMode DEFAULT_MODE = BassCrossfadeMode.Always;

        const int DEFAULT_PERIOD = 100;

        const BassCrossfadeType DEFAULT_TYPE = BassCrossfadeType.OutQuad;

        const bool DEFAULT_MIX = false;

        const bool DEFAULT_BUFFER = false;

        const string DllName = "bass_crossfade";

        public static int Module = 0;

        public static bool Load(string folderName = null)
        {
            if (Module == 0)
            {
                var fileName = default(string);
                if (!string.IsNullOrEmpty(folderName))
                {
                    fileName = Path.Combine(folderName, DllName);
                }
                else
                {
                    fileName = Path.Combine(Loader.FolderName, DllName);
                }
                Module = Bass.PluginLoad(string.Format("{0}.{1}", fileName, Loader.Extension));
            }
            return Module != 0;
        }

        public static bool Unload()
        {
            if (Module != 0)
            {
                if (!Bass.PluginFree(Module))
                {
                    return false;
                }
                Module = 0;
            }
            return true;
        }

        public static BassCrossfadeMode Mode
        {
            get
            {
                var mode = default(int);
                if (!GetConfig(BassCrossfadeAttribute.Mode, out mode))
                {
                    return DEFAULT_MODE;
                }
                return (BassCrossfadeMode)mode;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.Mode, Convert.ToInt32(value));
            }
        }

        public static int InPeriod
        {
            get
            {
                var period = default(int);
                if (!GetConfig(BassCrossfadeAttribute.InPeriod, out period))
                {
                    return DEFAULT_PERIOD;
                }
                return period;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.InPeriod, value);
            }
        }

        public static int OutPeriod
        {
            get
            {
                var period = default(int);
                if (!GetConfig(BassCrossfadeAttribute.OutPeriod, out period))
                {
                    return DEFAULT_PERIOD;
                }
                return period;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.OutPeriod, value);
            }
        }

        public static BassCrossfadeType InType
        {
            get
            {
                var type = default(int);
                if (!GetConfig(BassCrossfadeAttribute.InType, out type))
                {
                    return DEFAULT_TYPE;
                }
                return (BassCrossfadeType)type;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.InType, Convert.ToInt32(value));
            }
        }

        public static BassCrossfadeType OutType
        {
            get
            {
                var type = default(int);
                if (!GetConfig(BassCrossfadeAttribute.OutType, out type))
                {
                    return DEFAULT_TYPE;
                }
                return (BassCrossfadeType)type;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.OutType, Convert.ToInt32(value));
            }
        }

        public static bool Mix
        {
            get
            {
                var mix = default(int);
                if (!GetConfig(BassCrossfadeAttribute.Mix, out mix))
                {
                    return DEFAULT_MIX;
                }
                return mix != 0;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.Mix, value ? 1 : 0);
            }
        }

        public static bool Buffer
        {
            get
            {
                var buffer = default(int);
                if (!GetConfig(BassCrossfadeAttribute.Buffer, out buffer))
                {
                    return DEFAULT_BUFFER;
                }
                return buffer != 0;
            }
            set
            {
                SetConfig(BassCrossfadeAttribute.Buffer, value ? 1 : 0);
            }
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_SetConfig(BassCrossfadeAttribute Attrib, int Value);

        public static bool SetConfig(BassCrossfadeAttribute Attrib, int Value)
        {
            return BASS_CROSSFADE_SetConfig(Attrib, Value);
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_GetConfig(BassCrossfadeAttribute Attrib, out int Value);

        public static bool GetConfig(BassCrossfadeAttribute Attrib, out int Value)
        {
            return BASS_CROSSFADE_GetConfig(Attrib, out Value);
        }

        [DllImport(DllName)]
        static extern int BASS_CROSSFADE_StreamCreate(int Frequency, int Channels, BassFlags Flags, IntPtr User = default(IntPtr));

        public static int StreamCreate(int Frequency, int Channels, BassFlags Flags, IntPtr User = default(IntPtr))
        {
            return BASS_CROSSFADE_StreamCreate(Frequency, Channels, Flags, User);
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_StreamFadeIn();

        public static bool StreamFadeIn()
        {
            return BASS_CROSSFADE_StreamFadeIn();
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_StreamFadeOut();

        public static bool StreamFadeOut()
        {
            return BASS_CROSSFADE_StreamFadeOut();
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_StreamReset(BassCrossfadeFlags Flags);

        public static bool StreamReset(BassCrossfadeFlags Flags = BassCrossfadeFlags.Default)
        {
            return BASS_CROSSFADE_StreamReset(Flags);
        }

        [DllImport(DllName)]
        static extern IntPtr BASS_CROSSFADE_GetChannels(out int Count);

        public static int[] GetChannels(out int Count)
        {
            var channels = BASS_CROSSFADE_GetChannels(out Count);
            var result = new int[Count];
            Marshal.Copy(channels, result, 0, Count);
            return result;
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_ChannelEnqueue(int Handle, BassCrossfadeFlags Flags);

        public static bool ChannelEnqueue(int Handle, BassCrossfadeFlags Flags = BassCrossfadeFlags.Default)
        {
            return BASS_CROSSFADE_ChannelEnqueue(Handle, Flags);
        }

        [DllImport(DllName)]
        static extern bool BASS_CROSSFADE_ChannelRemove(int Handle, BassCrossfadeFlags Flags);

        public static bool ChannelRemove(int Handle, BassCrossfadeFlags Flags = BassCrossfadeFlags.Default)
        {
            return BASS_CROSSFADE_ChannelRemove(Handle, Flags);
        }
    }

    public enum BassCrossfadeAttribute
    {
        None = 0,
        Mixer = 1,
        Mode = 2,
        InPeriod = 3,
        OutPeriod = 4,
        InType = 5,
        OutType = 6,
        Mix = 7,
        Buffer = 8
    }

    public enum BassCrossfadeMode
    {
        None = 0,
        Always = 1,
        Manual = 2
    }

    public enum BassCrossfadeType
    {
        None = 0,
        Linear = 1,
        InQuad = 2,
        OutQuad = 3,
        InExpo = 4,
        OutExpo = 5
    }

    public enum BassCrossfadeFlags
    {
        Default = 0,
        None = 1,
        FadeIn = 2,
        FadeOut = 3
    }
}
