using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using ZedGraph;
using System.Threading;
namespace MotorControl
{
    public partial class MainForm : Form
    {  
        UInt16 COMBaudRate;
        uint COM_opended,Sending_enable;
        UInt64 i;
        string[] s;
        float setpoint;
        double speed, time;
        uint direction = 1;
        GraphPane myPane = new GraphPane();
        // point par lists
        PointPairList listPointOne = new PointPairList();
        PointPairList listPointTwo = new PointPairList();
        // line item
        LineItem myCurveOne;
        LineItem myCurveTwo;

        public MainForm()
        {
            InitializeComponent();
            
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            i = 0;
            time = 0;
            COMBaudRate = 38400;
            textBoxBaudrate.Text = COMBaudRate.ToString();
            
             s= System.IO.Ports.SerialPort.GetPortNames();
            foreach (string str in s)
            {
                listBoxPortName.Items.Add(str);
            }
            if (s.Length>0) { listBoxPortName.SelectedIndex = 0; }
            COM_opended = 0;

            checkBoxManual.CheckState=CheckState.Unchecked;
            textBoxPWMWidth.ReadOnly = true;
            buttonSetPWM.Enabled = false;
            textBoxKP.ReadOnly = true;
            textBoxTI.ReadOnly = true;
            textBoxTD.ReadOnly = true;
            buttonSetSP.Enabled = false;
            buttonSetKP.Enabled = false;
            buttonSetTI.Enabled = false;
            buttonSetTD.Enabled = false;
            textBoxSP.ReadOnly = true;
            buttonReceive.Enabled = false;
            //set pane
            ZedGraphControl z = new ZedGraphControl();
            myPane = zgc.GraphPane;
            myPane.Y2Axis.Scale.Min = 0;
            myPane.Y2Axis.Scale.Max = 25;
            myPane.Title.Text = "Motor speed"; 
            myPane.XAxis.Title.Text = "Time (s)";
            myPane.YAxis.Title.Text = "Speed (r/s)";
            myCurveOne = myPane.AddCurve("Speed", listPointOne, Color.Black, SymbolType.None);
            myCurveTwo = myPane.AddCurve("Setpoint", listPointTwo, Color.Blue, SymbolType.HDash);
            
            myCurveOne.Line.Width = 2;
            zgc.GraphPane.Legend.IsVisible = true;
            myCurveTwo.Line.Width = 3;
            
        }

        private void buttonOpenPort_Click(object sender, EventArgs e)
        {
            if (COM_opended == 0)
            {
                if(s.Length>0) serialPort1.PortName = listBoxPortName.Items[listBoxPortName.SelectedIndex].ToString();
                serialPort1.BaudRate = COMBaudRate;
                serialPort1.DataBits = 8;
                serialPort1.Parity = Parity.None;
                serialPort1.StopBits = StopBits.One;
                
                try { serialPort1.Open(); }
                catch 
                {   
                    MessageBox.Show("Can not open this serial port","Error",MessageBoxButtons.OK, MessageBoxIcon.Error);
                    COM_opended = 0;
                }
                if (serialPort1.IsOpen)
                {
                    COM_opended = 1;
                    MessageBox.Show("Successfully opened!");
                    buttonOpenPort.Text = "Disconnect";
                    buttonReceive.Enabled = true;
                    
                    textBoxBaudrate.ReadOnly = true;
                    if (checkBoxManual.CheckState == CheckState.Checked)
                    {
                        textBoxSP.ReadOnly = true;
                        textBoxKP.ReadOnly = true;
                        textBoxTI.ReadOnly = true;
                        textBoxTD.ReadOnly = true;
                        buttonSetSP.Enabled = false;
                        buttonSetKP.Enabled = false;
                        buttonSetTI.Enabled = false;
                        buttonSetTD.Enabled = false;
                        textBoxPWMWidth.ReadOnly = false;
                        buttonSetPWM.Enabled = true;
                    }
                    else
                    {
                        textBoxSP.ReadOnly = false;
                        textBoxKP.ReadOnly = false;
                        textBoxTI.ReadOnly = false;
                        textBoxTD.ReadOnly = false;
                        buttonSetSP.Enabled = true;
                        buttonSetKP.Enabled = true;
                        buttonSetTI.Enabled = true;
                        buttonSetTD.Enabled = true;
                        textBoxPWMWidth.ReadOnly = true;
                        buttonSetPWM.Enabled = false;
                    }

                }
                else COM_opended = 0;
            }
            else
            {
                buttonReceive.Enabled = false;
                textBoxBaudrate.ReadOnly = false;
                
                Sending_enable = 0;
                Thread.Sleep(100);
                serialPort1.Write("u0000");
                Thread.Sleep(100);
                serialPort1.Close();
                COM_opended = 0;
                buttonOpenPort.Text = "Open";
                textBoxSP.ReadOnly = true;
                textBoxKP.ReadOnly = true;
                textBoxTI.ReadOnly = true;
                textBoxTD.ReadOnly = true;
                buttonSetSP.Enabled = false;
                buttonSetKP.Enabled = false;
                buttonSetTI.Enabled = false;
                buttonSetTD.Enabled = false;
                textBoxPWMWidth.ReadOnly = true;
                buttonSetPWM.Enabled = false;
            }
        }

        private void MainForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            serialPort1.Close();
        }

        private void checkBoxManual_CheckStateChanged(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                if (checkBoxManual.CheckState == CheckState.Checked)
                {
                    textBoxSP.ReadOnly = true;
                    textBoxKP.ReadOnly = true;
                    textBoxTI.ReadOnly = true;
                    textBoxTD.ReadOnly = true;
                    buttonSetSP.Enabled = false;
                    buttonSetKP.Enabled = false;
                    buttonSetTI.Enabled = false;
                    buttonSetTD.Enabled = false;
                    textBoxPWMWidth.ReadOnly = false;
                    buttonSetPWM.Enabled = true;

                }
                else
                {
                    textBoxSP.ReadOnly = false;
                    textBoxKP.ReadOnly = false;
                    textBoxTI.ReadOnly = false;
                    textBoxTD.ReadOnly = false;
                    buttonSetSP.Enabled = true;
                    buttonSetKP.Enabled = true;
                    buttonSetTI.Enabled = true;
                    buttonSetTD.Enabled = true;
                    textBoxPWMWidth.ReadOnly = true;
                    buttonSetPWM.Enabled = false;
                    serialPort1.Write("a0000");
                }
            }
        }

        private void buttonSetParam_Click(object sender, EventArgs e)
        {
            string send_string;
            try
            {
                
                send_string = 's' + textBoxSP.Text;
                serialPort1.WriteLine(send_string);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message,"Error",MessageBoxButtons.OK,MessageBoxIcon.Error);
            }   
        }

        /*----------------------------Receive data and put to richtextbox and graph---------------------*/
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string s;       
                s = serialPort1.ReadLine();
                PutInRTBox(s);
                UpdateZGraph(zgc, s);
                UpdateParams(s);
        }

        private delegate void dlDisplay(string s);
        private void PutInRTBox(string s)
        {
            string newline = "";
            char n = '\n';
            newline=newline+n;
            if (richTextBox1.InvokeRequired)
            {
                dlDisplay dd = new dlDisplay(PutInRTBox);
                richTextBox1.Invoke(dd, new object[] { s });
            }
            else
            {
                if (Sending_enable == 1)
                {
                    if (s[0] != 'p')
                        richTextBox1.AppendText(s + System.Environment.NewLine);
                }
            }
        }
        delegate void axisChangeZedGraphCallBack(ZedGraphControl z,string s);
        private void UpdateZGraph(ZedGraphControl z,string s)
        {
            if (z.InvokeRequired)
            {
                axisChangeZedGraphCallBack dlz = new axisChangeZedGraphCallBack(UpdateZGraph);
                z.Invoke(dlz, new object [] { z,s });
            }
            else
            {
                int a, b, c;
                if (Sending_enable == 1)
                {
                    if (s[0] != 'p')
                    {
                        a = s[0] - 48; b = s[1] - 48; c = s[3] - 48;
                        speed = 10 * (double)a + (double)b + 0.1 * (double)c;
                        time = time+ 0.025;
                        myCurveTwo.AddPoint(time, setpoint); 
                        myCurveOne.AddPoint(time, speed);
                        z.AxisChange();
                        //z.Invalidate();
                        z.Refresh();
                    }
                }
            }
        }

        private delegate void dlUpdateParams(string s);
        private void UpdateParams(string s)
        {
            double val;
            string val_str="";
            if (textBoxSP.InvokeRequired)
            {
                dlUpdateParams ds = new dlUpdateParams(UpdateParams);
                textBoxSP.Invoke(ds, new object[] { s });
            }
            if (textBoxKP.InvokeRequired)
            {
                dlUpdateParams dp = new dlUpdateParams(UpdateParams);
                textBoxKP.Invoke(dp, new object[] { s });
            }
            if (textBoxTI.InvokeRequired)
            {
                dlUpdateParams di = new dlUpdateParams(UpdateParams);
                textBoxTI.Invoke(di, new object[] { s });
            }
            if (textBoxTD.InvokeRequired)
            {
                dlUpdateParams dd = new dlUpdateParams(UpdateParams);
                textBoxSP.Invoke(dd, new object[] { s });
            }
            
                if (s[0] == 'p')
                {
                    val_str = val_str + s[1] + s[2] +','+ s[3] + s[4];
                    val = UART_Param_Convert(val_str);
                    setpoint = float.Parse(val_str);
                    textBoxSP.Text = val_str;

                    val_str = "";
                    val_str = val_str + s[5] + s[6] + ',' + s[7] + s[8];
                    textBoxKP.Text = val_str;

                    val_str = "";
                    val_str = val_str + s[9] + s[10] + ',' + s[11] + s[12];
                    textBoxTI.Text = val_str;

                    val_str = "";
                    val_str = val_str + s[13] + s[14] + ',' + s[15] + s[16];
                    textBoxTD.Text = val_str;
                }

            
        }
        /*------------------------------------------------------------------------------------------------*/
        private void buttonSetSP_Click(object sender, EventArgs e)
        {
            string s;
            //float setpoint;
            int a, b, c, d;
           
            try
            { setpoint = float.Parse(textBoxSP.Text);
            a = (int)(setpoint / 10);
            b = (int)(setpoint - 10 * a);
            c = (int)((setpoint - 10 * a - b) * 10);
            d = (int)((setpoint - 10 * a - b - 0.1 * c) * 100);

            s = 's' + a.ToString() + b.ToString() +c.ToString() + d.ToString();
             serialPort1.Write(s);
           
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
               
            }
    
        }

        private void buttonSetKP_Click(object sender, EventArgs e)
        {
            string s;
            float setpoint;
            int a, b, c, d;
            try
            {
                setpoint = float.Parse(textBoxKP.Text);
                a = (int)(setpoint / 10);
                b = (int)(setpoint - 10 * a);
                c = (int)((setpoint - 10 * a - b) * 10);
                d = (int)((setpoint - 10 * a - b - 0.1 * c) * 100);

                s = 'p' + a.ToString() + b.ToString() + c.ToString() + d.ToString();
                serialPort1.Write(s);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }
        }

        private void buttonSetTI_Click(object sender, EventArgs e)
        {
            string s;
            float setpoint;
            int a, b, c, d;
            try
            {
                setpoint = float.Parse(textBoxTI.Text);
                a = (int)(setpoint / 10);
                b = (int)(setpoint - 10 * a);
                c = (int)((setpoint - 10 * a - b) * 10);
                d = (int)((setpoint - 10 * a - b - 0.1 * c) * 100);

                s = 'i' + a.ToString() + b.ToString() + c.ToString() + d.ToString();
                serialPort1.Write(s);

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }

        }

        private void buttonSetTD_Click(object sender, EventArgs e)
        {
            string s;
            float setpoint;
            int a, b, c, d;
            try
            {
                setpoint = float.Parse(textBoxTD.Text);
                a = (int)(setpoint / 10);
                b = (int)(setpoint - 10 * a);
                c = (int)((setpoint - 10 * a - b) * 10);
                d = (int)((setpoint - 10 * a - b - 0.1 * c) * 100);

                s = 'd' + a.ToString() + b.ToString() + c.ToString() + d.ToString();
                serialPort1.Write(s);

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }
        }

        private void buttonSetPWM_Click(object sender, EventArgs e)
        {
            string s;
            float setpoint;
            int a, b, c;
            try
            {
                setpoint = float.Parse(textBoxPWMWidth.Text);
                if(setpoint>=100) s="m1000";
                else
                {
                    a = (int)(setpoint / 10);
                    b = (int)(setpoint - 10 * a);
                    c = (int)(10 * (setpoint - 10 * a - b));
                    s = "m" +"0"+ a.ToString() + b.ToString() + c.ToString();
                   
                }
                serialPort1.Write(s);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);

            }

        }

        private void buttonReceive_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {
                if (Sending_enable == 0)
                {
                    Sending_enable = 1;
                    serialPort1.Write("u1000");
                    buttonReceive.Text = "Stop receiving";
                }
                else
                {
                    Sending_enable = 0;
                    serialPort1.Write("u0000");
                    buttonReceive.Text = "Receive data";
                }
                //serialPort1.Write("r0000"); //request
            }
            
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }


        private void buttonClearData_Click(object sender, EventArgs e)
        {
            time = 0;
            richTextBox1.Clear();
            myCurveOne.Clear();
            myCurveTwo.Clear();
            myPane.XAxis.Scale.Min = 0;
            myPane.Y2Axis.Scale.Min = 0;
            myPane.Y2Axis.Scale.Max = 25;
            myPane.AxisChange();
        }


        private void saveDataToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog saveFileDialog1 = new SaveFileDialog();
            saveFileDialog1.Filter = "rtf files (*.rtf)|*.rtf|All files (*.*)|*.*";
            saveFileDialog1.FilterIndex = 2;
            saveFileDialog1.RestoreDirectory = true;
            if (saveFileDialog1.ShowDialog() == DialogResult.OK)
            {

                System.IO.StreamWriter SW = new System.IO.StreamWriter(saveFileDialog1.FileName, false, Encoding.ASCII);
                richTextBox1.SaveFile(saveFileDialog1.FileName);
                SW.Close();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == true)
            {
                serialPort1.Write("r0000"); //request
            }
        }
        /*Convert string abcd to float ab.cd*/
        private double UART_Param_Convert(string s)
        {
            double val;
            int a, b, c, d;
            a = s[0] - 48;
            b = s[1] - 48;
            c = s[2] - 48;
            d = s[3] - 48;
            val = ((double)a) * 10 + ((double)b) + ((double)c) * 0.1 + ((double)d) * 0.01;
            return val;
        }

        private void buttonDirection_Click(object sender, EventArgs e)
        {   
            if (serialPort1.IsOpen == true)
            {
                if (direction == 1)
                {
                    serialPort1.Write("n0000");
                    direction = 0;
                }
                else
                {
                    serialPort1.Write("n1000");
                    direction = 1;
                }
            }
        }
        
    }
}

