using System;
using System.Text;
using System.IO;
using System.IO.Pipes;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Windows;

namespace ServerGUI
{

    class ServerController
    {
        private NamedPipeClientStream pipeClient_ = null;
        private StreamReader reader_ = null;
        
        private int bufSize_;
        private byte[] buf_;



        public ServerController(string name, int bufSize = 0, int timeout_ms = 0)
        {
            bufSize_ = bufSize;
            buf_ = new byte[bufSize_];

            pipeClient_ = new NamedPipeClientStream(".", name, PipeDirection.InOut);

            reader_ = new StreamReader(pipeClient_);

            pipeClient_.Connect(timeout_ms);
        }



        public void sendQuery(Query query)
        {
            pipeClient_.WriteByte((byte)query);
        }



        public void sendData(uint num)
        {
            pipeClient_.Write(BitConverter.GetBytes(num), 0, sizeof(uint));
        }



        public byte[] receiveBytes()
        {
            int size_35mb = 1024 * 1024 * 35;
            byte[] bufImg = new byte[size_35mb];

            int bytes = pipeClient_.Read(bufImg, 0, size_35mb);
            Array.Resize(ref bufImg, bytes);
            return bufImg;
        }



        public string receiveData()
        {
            Array.Clear(buf_, 0, buf_.Length);
            pipeClient_.Read(buf_, 0, bufSize_);
            return Encoding.UTF8.GetString(buf_);
        }



        public string receiveDataUnicode()
        {
            Array.Clear(buf_, 0, buf_.Length);
            pipeClient_.Read(buf_, 0, bufSize_);
            Encoding utf16LE = Encoding.GetEncoding("UTF-16LE");
            return utf16LE.GetString(buf_);
        }

    }
}
