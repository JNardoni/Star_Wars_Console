using System;
using System.Collections.Generic;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO.Ports;
using System.Media;
using Timer = System.Timers.Timer;


namespace StarWarsControl
{
    public partial class Form1 : Form
    {
        int CURRENT_MODE = 0;

        int ct = 0;

        //--Communication with Arduino--
        SerialPort sPort;

        //--Playing Sounds--
        System.Media.SoundPlayer r2player; //Player
        int soundPlaying = 0;         //Flag - is something playing?
        int lastRadomSound;           //Last Random Sound played, to reduce duplicates

        Timer r2Timer = new Timer();

        //--Hyperdrive info--
        int leverPos = 0;     //current position of the hyperdrive lever
        const int leverMax = 10;    //Max value of hyperdrive lever
        const int leverMin = 0;     //Min value of the hyperdrive lever
                             
        public Form1()
        {
            // ---Form Seetup---
            InitializeComponent();

            // progressBar1.Click += new EventHandler(this.onClick);
            btncom2.FlatAppearance.BorderColor = Color.SlateGray;
            //Touchscreen, hide cursor
            // Cursor.Hide(); 


            //---Init Buttons---
            btncom1.Click += new EventHandler(this.btn1onClick);
            btncom2.Click += new EventHandler(this.btn1onClick);
            btncom3.Click += new EventHandler(this.btn1onClick);

            btnshld1.Click += new EventHandler(this.btn1onClick);
            btnshld2.Click += new EventHandler(this.btn1onClick);
            btnshld3.Click += new EventHandler(this.btn1onClick);


            btnengine1.Click += new EventHandler(this.btn1onClick);
            btnengine2.Click += new EventHandler(this.btn1onClick);
            btnengine3.Click += new EventHandler(this.btn1onClick);
            btnengine4.Click += new EventHandler(this.btn1onClick);
            btnengine5.Click += new EventHandler(this.btn1onClick);
            btnengine6.Click += new EventHandler(this.btn1onClick);
            btnengine7.Click += new EventHandler(this.btn1onClick);
            btnengine8.Click += new EventHandler(this.btn1onClick);


            btnhyper1.Click += new EventHandler(this.btn1onClick);
            btnhyper2.Click += new EventHandler(this.btn1onClick);
            btnhyper3.Click += new EventHandler(this.btn1onClick);
            btnhyper4.Click += new EventHandler(this.btn1onClick);
            btnhyper5.Click += new EventHandler(this.btn1onClick);
            btnhyper6.Click += new EventHandler(this.btn1onClick);
            btnhyper7.Click += new EventHandler(this.btn1onClick);
            btnhyper8.Click += new EventHandler(this.btn1onClick);

            btnidk1.Click += new EventHandler(this.playNoise);


            //---Init Serial com ports --- (port 6: PC, ttyACM0: Raspberry pi)
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


            //---Init Timer for R2 Sounds---
            //R2Timer. += new ElapsedEventHandler(timerTriggered);
            r2Timer.Interval = 2000;
            r2Timer.Elapsed += timerTriggered;
            r2Timer.Start();


        }

        void btn1onClick(Object sender, EventArgs e)
        {
            Button b = (Button)sender;

            if (b.BackColor == Color.AliceBlue)
            {
                Random rnd = new Random();
                int c = rnd.Next(0, 2);
                if (c == 0)
                {
                    b.BackColor = Color.SkyBlue;
                }
                else
                {
                    b.BackColor = Color.RoyalBlue;
                }
            }

            else
            {
                b.BackColor = Color.AliceBlue;
            }

        }

        void reset(Object sender, EventArgs e)
        {
            if (sPort.IsOpen)
            {
                sPort.Write("R");
            }
        }

        void turnOn(Object sender, EventArgs e)
        {
            if (sPort.IsOpen)
            {
                sPort.Write("S");
            }
        }

        void turnOff(Object sender, EventArgs e)
        {
            if (sPort.IsOpen)
            {
                sPort.Write("O");
            }
        }

        private void timerTriggered(Object sender, System.Timers.ElapsedEventArgs e)
        {
            Random rnd = new Random();
            int tmr = rnd.Next(30000, 120000);
            r2Timer.Interval = tmr;
            
            int sound = rnd.Next(0, 7);

            switch (sound) { 
                case 0:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\UnconvincedGrumbling.wav");
                    break;
                case 1:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\Lonely_singing.wav");
                    break;
                case 2:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\Lonely_Hello.wav");
                    break;
                case 3:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\SingSongResponse.wav");
                    break;
                case 4:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\Happy_Confirmation.wav");
                    break;
                case 5:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\R2D2a.wav");
                    break;
                case 6:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\StifledLaugh.wav");
                    break;
                default:
                    break;
            }
            

            r2player.Play();

        }


        //When told to play a noise
        private void playNoise(Object sender, EventArgs e)
        {
            Random rnd = new Random();
            int sound = rnd.Next(0, 3);

            switch (sound)
            {
                case 0:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\CurtReply.wav");
                    break;
                case 1:
                    r2player = new System.Media.SoundPlayer(@"C:\Users\Jakid\Desktop\TestyBlowup.wav");
                    break;
                case 2:
                    r2player = new System.Media.SoundPlayer(@"C: \Users\Jakid\Desktop\Happy_ThreeChirp.wav");
                    break;
                default:
                    break;
            }
            r2player.Play();
        }

        private void randomR2Sound()
        {



        }


        //Recieves data from the connecting serial port
        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            string data = sPort.ReadLine();     

            ct = Convert.ToInt32(data);

        }

    }
}
