using System;
using System.Collections.Generic;
using System.Text;

namespace H264Net.Encoder
{
    public class H264NetEncodingError : Exception
    {
        public H264NetEncodingError(string message) : base(message)
        {
        }       
    }

    public class H264NetForceIDRError : Exception
    {
        public H264NetForceIDRError(string message) : base(message)
        {

        }
    }

    public class H264NetFramebufferError : Exception
    {
        public H264NetFramebufferError(string message) : base(message)
        {

        }

        public H264NetFramebufferError(string message, Exception innerException) : base(message, innerException)
        {
        }
    }
}
