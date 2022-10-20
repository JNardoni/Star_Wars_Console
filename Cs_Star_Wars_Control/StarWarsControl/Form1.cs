using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Drawing.Drawing2D;
using System.Windows.Forms;
using System.Linq;
using System.Text;
using System.Threading;

namespace StarWarsControl
{
    public partial class Form1 : Form
    {
        int CURRENT_MODE = 0;
        int ledCount = 3;

        int ct = 0;

        SerialPort sPort;
        String portName;
                        
        public Form1()
        {

            InitializeComponent();

            // progressBar1.Click += new EventHandler(this.onClick);
            btncom2.FlatAppearance.BorderColor = Color.SlateGray;

            btncom2.Click += new EventHandler(this.btn1onClick);
            button2_1.Click += new EventHandler(this.btn1onClick);
            btncom3.Click += new EventHandler(this.btn1onClick);
            btncom1.Click += new EventHandler(this.btn1onClick);
            button3_1.Click += new EventHandler(this.btn1onClick);
            button3_2.Click += new EventHandler(this.btn1onClick);

            //Touchscreen, hide cursor
            // Cursor.Hide(); 
           // sPort = new SerialPort("/dev/ttyACM0", 9600);
            sPort = new SerialPort("COM6", 9600);
            sPort.DataReceived += serialPort1_DataReceived;

            try
            {
                sPort.Open();
                btncom2.Text = "1open";
            }

            catch
            {
                btncom2.Text = "";
            }

        }

        void btn1onClick(Object sender, EventArgs e)
        {
            Button b = (Button)sender;


            if (b.BackColor == Color.AliceBlue)
            {
                b.BackColor = Color.Red;
                if (sPort.IsOpen)
                {
                    sPort.Write("L");
                }
            }

            else
            {
                b.BackColor = Color.AliceBlue;


                if (sPort.IsOpen)
                {
                    sPort.Write("L");
                }
            }

        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string data = sPort.ReadLine();     

            ct = Convert.ToInt32(data);

        }

    }
}
