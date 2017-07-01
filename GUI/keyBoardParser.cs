using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace HookAPP
{
    class keyBoardParser
    {
        private const int keyNumIndex= 0x30;
        public static string parseKey(int key)
        {
            string retval = "";

            if(keyIsNumber(key))
            {
                retval = keyNumParse(key);
            }
            else if (keyIsAlpha(key))
            {
                retval = keyAlphaParse(key);
            }
            else if (key == 0xd)
            {
                retval = "Enter";
            }
            else
            {
                retval = "Unknown";
            }

            return retval;
        }

        public static bool keyIsNumber(int key)
        {
            if (key >= 0x30 && key <= 0x39)
            {
                return true;
            }
            return false;
        }
        public static bool keyIsAlpha(int key)
        {
            if (key >= 0x41 && key <= 0x5a)
            {
                return true;
            }
            return false;
        }
        public static string keyNumParse(int key)
        {
            return (key - keyNumIndex).ToString();
        }
        public static string keyAlphaParse(int key)
        {
            return Convert.ToChar(key).ToString();
        }
    }
}
