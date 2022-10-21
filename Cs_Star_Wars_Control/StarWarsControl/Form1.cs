using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Media;

namespace StarWarsControl
{
    public partial class Form1 : Form
    {
        int CURRENT_MODE = 0;
        int ledCount = 3;

        int ct = 0;

        //Communication with Arduino
        SerialPort sPort;
        String portName;

        //Playing Sounds
        System.Media.SoundPlayer r2player;
        System.Media.SoundPlayer enginePlayer;


        int soundPlaying = 0;
                     
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
            button5_1.Click += new EventHandler(this.playNoise);


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

        private void playNoise(Object sender, EventArgs e)
        {
               r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Downloads\R2D2a.wav");
               r2player.PlayLooping();

               enginePlayer = new System.Media.SoundPlayer(@"C:\Users\Jakid\Downloads\Lonely_Hello.wav");
               enginePlayer.PlayLooping();

        }


        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string data = sPort.ReadLine();     

            ct = Convert.ToInt32(data);

        }

    }
}
