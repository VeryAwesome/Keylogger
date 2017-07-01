using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Threading;

namespace HookAPP
{
    public partial class Form1 : Form
    {
        public dllHandler.updateListDelegate myDelegate;
        public Form1()
        {
            InitializeComponent();
            this.FormClosing +=new FormClosingEventHandler(this.closeForm);
            myDelegate = new dllHandler.updateListDelegate(updateList);
            Thread t = new Thread(() => dllHandler.KeyLogThread(this));
            t.Start();
        }
        public void closeForm(object sender, FormClosingEventArgs e)
        {
            dllHandler.running = false;
            dllHandler.set_exit(1);
        }
        public void updateList(int index, string key, int value)
        {
            ListViewItem myItem = new ListViewItem(index.ToString());
            myItem.SubItems.Add(key);
            myItem.SubItems.Add(value.ToString());
            listView1.Items.Add(myItem);
        }
    }
}
