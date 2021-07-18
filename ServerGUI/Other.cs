using System.Collections.Generic;

namespace ServerGUI
{
    enum Query : byte
    {
        ConnectToPC1     = 1,
        ConnectToPC2     = 2,
        ConnectToPC3     = 3,
        ConnectToPC4     = 4,
        ConnectToPC5     = 5,
        ConnectToPC6     = 6,
        GetKeyLog        = 8,
        GetProcessInfo   = 9,
        TerminateProcess = 10,
        GetScreenshot    = 11,
        ShutdownPC       = 12,
        LogoutPC         = 13,
        RebootPC         = 14,
        GetNetLog        = 15
    };

    public class ProcessEntry32
    {
        public uint dwSize { get; set; }
        public uint cntUsage { get; set; }
        public uint th32ProcessID { get; set; }
        public int th32DefaultHeapID { get; set; }
        public uint th32ModuleID { get; set; }
        public uint cntThreads { get; set; }
        public uint th32ParentProcessID { get; set; }
        public int pcPriClassBase { get; set; }
        public uint dwFlags { get; set; }
        public List<byte> szExeFile { get; set; }
    }

    public class ProcessInfo
    {
        public string name { get; set; }
        public uint pid { get; set; }
        public uint cntThreads { get; set; }

        public override string ToString()
        {
            return name + " " + pid + " " + cntThreads;
        }
    }
}