using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace HookAPP
{
    public class dllHandler
    {
        public static bool running = true;

        public delegate void updateListDelegate(int index, string key, int value);

        [DllImport("C:\\Users\\Very Awesome\\Documents\\visual studio 2012\\Projects\\hooks\\Debug\\hooks.dll")]
        public static extern int get_count();

        [DllImport("C:\\Users\\Very Awesome\\Documents\\visual studio 2012\\Projects\\hooks\\Debug\\hooks.dll")]
        public static extern int get_last_key();

        [DllImport("C:\\Users\\Very Awesome\\Documents\\visual studio 2012\\Projects\\hooks\\Debug\\hooks.dll")]
        public static extern int set_exit(int num);


        public static void KeyLogThread(Form1 form)
        {
            int index = 0;
            while (running == true)
            {
                int key = dllHandler.get_last_key();
                if (key != 0)
                {
                    string keyString = keyBoardParser.parseKey(key);
                    form.Invoke(form.myDelegate, index, keyString, key);
                    index++;
                }
            }
        }
    }
}
