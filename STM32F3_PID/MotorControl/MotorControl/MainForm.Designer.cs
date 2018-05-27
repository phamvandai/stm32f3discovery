namespace MotorControl
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.buttonOpenPort = new System.Windows.Forms.Button();
            this.groupBoxCOMCfg = new System.Windows.Forms.GroupBox();
            this.buttonReceive = new System.Windows.Forms.Button();
            this.listBoxPortName = new System.Windows.Forms.ListBox();
            this.textBoxBaudrate = new System.Windows.Forms.TextBox();
            this.labelCOMBaudRate = new System.Windows.Forms.Label();
            this.labelCOMName = new System.Windows.Forms.Label();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.groupBoxControl = new System.Windows.Forms.GroupBox();
            this.groupBoxManual = new System.Windows.Forms.GroupBox();
            this.buttonSetPWM = new System.Windows.Forms.Button();
            this.textBoxPWMWidth = new System.Windows.Forms.TextBox();
            this.labelPWMPulse = new System.Windows.Forms.Label();
            this.checkBoxManual = new System.Windows.Forms.CheckBox();
            this.groupBoxPID = new System.Windows.Forms.GroupBox();
            this.buttonGetParam = new System.Windows.Forms.Button();
            this.buttonSetTD = new System.Windows.Forms.Button();
            this.buttonSetTI = new System.Windows.Forms.Button();
            this.buttonSetKP = new System.Windows.Forms.Button();
            this.buttonSetSP = new System.Windows.Forms.Button();
            this.textBoxSP = new System.Windows.Forms.TextBox();
            this.labelSetpoint = new System.Windows.Forms.Label();
            this.textBoxKP = new System.Windows.Forms.TextBox();
            this.textBoxTI = new System.Windows.Forms.TextBox();
            this.textBoxTD = new System.Windows.Forms.TextBox();
            this.labelKP = new System.Windows.Forms.Label();
            this.labelTI = new System.Windows.Forms.Label();
            this.labelTD = new System.Windows.Forms.Label();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.zgc = new ZedGraph.ZedGraphControl();
            this.buttonClearData = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.label1 = new System.Windows.Forms.Label();
            this.buttonDirection = new System.Windows.Forms.Button();
            this.menuStrip1.SuspendLayout();
            this.groupBoxCOMCfg.SuspendLayout();
            this.groupBoxControl.SuspendLayout();
            this.groupBoxManual.SuspendLayout();
            this.groupBoxPID.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1149, 33);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.saveDataToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(50, 29);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // saveDataToolStripMenuItem
            // 
            this.saveDataToolStripMenuItem.Name = "saveDataToolStripMenuItem";
            this.saveDataToolStripMenuItem.Size = new System.Drawing.Size(161, 30);
            this.saveDataToolStripMenuItem.Text = "Save data";
            this.saveDataToolStripMenuItem.Click += new System.EventHandler(this.saveDataToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(161, 30);
            this.exitToolStripMenuItem.Text = "Exit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // buttonOpenPort
            // 
            this.buttonOpenPort.Location = new System.Drawing.Point(10, 105);
            this.buttonOpenPort.Name = "buttonOpenPort";
            this.buttonOpenPort.Size = new System.Drawing.Size(121, 35);
            this.buttonOpenPort.TabIndex = 1;
            this.buttonOpenPort.Text = "Open";
            this.buttonOpenPort.UseVisualStyleBackColor = true;
            this.buttonOpenPort.Click += new System.EventHandler(this.buttonOpenPort_Click);
            // 
            // groupBoxCOMCfg
            // 
            this.groupBoxCOMCfg.Controls.Add(this.buttonReceive);
            this.groupBoxCOMCfg.Controls.Add(this.listBoxPortName);
            this.groupBoxCOMCfg.Controls.Add(this.textBoxBaudrate);
            this.groupBoxCOMCfg.Controls.Add(this.labelCOMBaudRate);
            this.groupBoxCOMCfg.Controls.Add(this.labelCOMName);
            this.groupBoxCOMCfg.Controls.Add(this.buttonOpenPort);
            this.groupBoxCOMCfg.Location = new System.Drawing.Point(12, 47);
            this.groupBoxCOMCfg.Name = "groupBoxCOMCfg";
            this.groupBoxCOMCfg.Size = new System.Drawing.Size(276, 146);
            this.groupBoxCOMCfg.TabIndex = 2;
            this.groupBoxCOMCfg.TabStop = false;
            this.groupBoxCOMCfg.Text = "Configure serial port";
            // 
            // buttonReceive
            // 
            this.buttonReceive.Location = new System.Drawing.Point(137, 105);
            this.buttonReceive.Name = "buttonReceive";
            this.buttonReceive.Size = new System.Drawing.Size(122, 35);
            this.buttonReceive.TabIndex = 10;
            this.buttonReceive.Text = "Receive data";
            this.buttonReceive.UseVisualStyleBackColor = true;
            this.buttonReceive.Click += new System.EventHandler(this.buttonReceive_Click);
            // 
            // listBoxPortName
            // 
            this.listBoxPortName.FormattingEnabled = true;
            this.listBoxPortName.ItemHeight = 20;
            this.listBoxPortName.Location = new System.Drawing.Point(137, 33);
            this.listBoxPortName.Name = "listBoxPortName";
            this.listBoxPortName.Size = new System.Drawing.Size(122, 24);
            this.listBoxPortName.TabIndex = 9;
            // 
            // textBoxBaudrate
            // 
            this.textBoxBaudrate.Location = new System.Drawing.Point(137, 66);
            this.textBoxBaudrate.Name = "textBoxBaudrate";
            this.textBoxBaudrate.Size = new System.Drawing.Size(122, 26);
            this.textBoxBaudrate.TabIndex = 4;
            // 
            // labelCOMBaudRate
            // 
            this.labelCOMBaudRate.AutoSize = true;
            this.labelCOMBaudRate.Location = new System.Drawing.Point(6, 72);
            this.labelCOMBaudRate.Name = "labelCOMBaudRate";
            this.labelCOMBaudRate.Size = new System.Drawing.Size(79, 20);
            this.labelCOMBaudRate.TabIndex = 3;
            this.labelCOMBaudRate.Text = "Baudrate:";
            // 
            // labelCOMName
            // 
            this.labelCOMName.AutoSize = true;
            this.labelCOMName.Location = new System.Drawing.Point(6, 37);
            this.labelCOMName.Name = "labelCOMName";
            this.labelCOMName.Size = new System.Drawing.Size(100, 20);
            this.labelCOMName.TabIndex = 2;
            this.labelCOMName.Text = "Port number:";
            // 
            // serialPort1
            // 
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // groupBoxControl
            // 
            this.groupBoxControl.Controls.Add(this.groupBoxManual);
            this.groupBoxControl.Controls.Add(this.checkBoxManual);
            this.groupBoxControl.Controls.Add(this.groupBoxPID);
            this.groupBoxControl.Location = new System.Drawing.Point(12, 199);
            this.groupBoxControl.Name = "groupBoxControl";
            this.groupBoxControl.Size = new System.Drawing.Size(276, 435);
            this.groupBoxControl.TabIndex = 3;
            this.groupBoxControl.TabStop = false;
            this.groupBoxControl.Text = "Control";
            // 
            // groupBoxManual
            // 
            this.groupBoxManual.Controls.Add(this.buttonSetPWM);
            this.groupBoxManual.Controls.Add(this.textBoxPWMWidth);
            this.groupBoxManual.Controls.Add(this.labelPWMPulse);
            this.groupBoxManual.Location = new System.Drawing.Point(6, 322);
            this.groupBoxManual.Name = "groupBoxManual";
            this.groupBoxManual.Size = new System.Drawing.Size(270, 108);
            this.groupBoxManual.TabIndex = 10;
            this.groupBoxManual.TabStop = false;
            this.groupBoxManual.Text = "Manual Mode";
            // 
            // buttonSetPWM
            // 
            this.buttonSetPWM.Location = new System.Drawing.Point(69, 64);
            this.buttonSetPWM.Name = "buttonSetPWM";
            this.buttonSetPWM.Size = new System.Drawing.Size(147, 38);
            this.buttonSetPWM.TabIndex = 6;
            this.buttonSetPWM.Text = "Set";
            this.buttonSetPWM.UseVisualStyleBackColor = true;
            this.buttonSetPWM.Click += new System.EventHandler(this.buttonSetPWM_Click);
            // 
            // textBoxPWMWidth
            // 
            this.textBoxPWMWidth.Location = new System.Drawing.Point(122, 32);
            this.textBoxPWMWidth.Name = "textBoxPWMWidth";
            this.textBoxPWMWidth.Size = new System.Drawing.Size(94, 26);
            this.textBoxPWMWidth.TabIndex = 5;
            // 
            // labelPWMPulse
            // 
            this.labelPWMPulse.AutoSize = true;
            this.labelPWMPulse.Location = new System.Drawing.Point(0, 38);
            this.labelPWMPulse.Name = "labelPWMPulse";
            this.labelPWMPulse.Size = new System.Drawing.Size(116, 20);
            this.labelPWMPulse.TabIndex = 0;
            this.labelPWMPulse.Text = "PWM width(%):";
            // 
            // checkBoxManual
            // 
            this.checkBoxManual.AutoSize = true;
            this.checkBoxManual.Location = new System.Drawing.Point(14, 21);
            this.checkBoxManual.Name = "checkBoxManual";
            this.checkBoxManual.Size = new System.Drawing.Size(131, 24);
            this.checkBoxManual.TabIndex = 9;
            this.checkBoxManual.Text = "Manual mode";
            this.checkBoxManual.UseVisualStyleBackColor = true;
            this.checkBoxManual.CheckStateChanged += new System.EventHandler(this.checkBoxManual_CheckStateChanged);
            // 
            // groupBoxPID
            // 
            this.groupBoxPID.Controls.Add(this.buttonDirection);
            this.groupBoxPID.Controls.Add(this.buttonGetParam);
            this.groupBoxPID.Controls.Add(this.buttonSetTD);
            this.groupBoxPID.Controls.Add(this.buttonSetTI);
            this.groupBoxPID.Controls.Add(this.buttonSetKP);
            this.groupBoxPID.Controls.Add(this.buttonSetSP);
            this.groupBoxPID.Controls.Add(this.textBoxSP);
            this.groupBoxPID.Controls.Add(this.labelSetpoint);
            this.groupBoxPID.Controls.Add(this.textBoxKP);
            this.groupBoxPID.Controls.Add(this.textBoxTI);
            this.groupBoxPID.Controls.Add(this.textBoxTD);
            this.groupBoxPID.Controls.Add(this.labelKP);
            this.groupBoxPID.Controls.Add(this.labelTI);
            this.groupBoxPID.Controls.Add(this.labelTD);
            this.groupBoxPID.Location = new System.Drawing.Point(6, 51);
            this.groupBoxPID.Name = "groupBoxPID";
            this.groupBoxPID.Size = new System.Drawing.Size(253, 265);
            this.groupBoxPID.TabIndex = 4;
            this.groupBoxPID.TabStop = false;
            this.groupBoxPID.Text = "PID parameters";
            // 
            // buttonGetParam
            // 
            this.buttonGetParam.Location = new System.Drawing.Point(69, 187);
            this.buttonGetParam.Name = "buttonGetParam";
            this.buttonGetParam.Size = new System.Drawing.Size(147, 33);
            this.buttonGetParam.TabIndex = 7;
            this.buttonGetParam.Text = "Get params";
            this.buttonGetParam.UseVisualStyleBackColor = true;
            this.buttonGetParam.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonSetTD
            // 
            this.buttonSetTD.Location = new System.Drawing.Point(186, 146);
            this.buttonSetTD.Name = "buttonSetTD";
            this.buttonSetTD.Size = new System.Drawing.Size(67, 35);
            this.buttonSetTD.TabIndex = 11;
            this.buttonSetTD.Text = "Set";
            this.buttonSetTD.UseVisualStyleBackColor = true;
            this.buttonSetTD.Click += new System.EventHandler(this.buttonSetTD_Click);
            // 
            // buttonSetTI
            // 
            this.buttonSetTI.Location = new System.Drawing.Point(186, 112);
            this.buttonSetTI.Name = "buttonSetTI";
            this.buttonSetTI.Size = new System.Drawing.Size(67, 32);
            this.buttonSetTI.TabIndex = 10;
            this.buttonSetTI.Text = "Set";
            this.buttonSetTI.UseVisualStyleBackColor = true;
            this.buttonSetTI.Click += new System.EventHandler(this.buttonSetTI_Click);
            // 
            // buttonSetKP
            // 
            this.buttonSetKP.Location = new System.Drawing.Point(186, 71);
            this.buttonSetKP.Name = "buttonSetKP";
            this.buttonSetKP.Size = new System.Drawing.Size(67, 33);
            this.buttonSetKP.TabIndex = 9;
            this.buttonSetKP.Text = "Set";
            this.buttonSetKP.UseVisualStyleBackColor = true;
            this.buttonSetKP.Click += new System.EventHandler(this.buttonSetKP_Click);
            // 
            // buttonSetSP
            // 
            this.buttonSetSP.Location = new System.Drawing.Point(186, 30);
            this.buttonSetSP.Name = "buttonSetSP";
            this.buttonSetSP.Size = new System.Drawing.Size(67, 35);
            this.buttonSetSP.TabIndex = 8;
            this.buttonSetSP.Text = "Set";
            this.buttonSetSP.UseVisualStyleBackColor = true;
            this.buttonSetSP.Click += new System.EventHandler(this.buttonSetSP_Click);
            // 
            // textBoxSP
            // 
            this.textBoxSP.Location = new System.Drawing.Point(107, 34);
            this.textBoxSP.Name = "textBoxSP";
            this.textBoxSP.Size = new System.Drawing.Size(73, 26);
            this.textBoxSP.TabIndex = 4;
            // 
            // labelSetpoint
            // 
            this.labelSetpoint.AutoSize = true;
            this.labelSetpoint.Location = new System.Drawing.Point(6, 37);
            this.labelSetpoint.Name = "labelSetpoint";
            this.labelSetpoint.Size = new System.Drawing.Size(104, 20);
            this.labelSetpoint.TabIndex = 0;
            this.labelSetpoint.Text = "Setpoint (r/s):";
            // 
            // textBoxKP
            // 
            this.textBoxKP.Location = new System.Drawing.Point(107, 74);
            this.textBoxKP.Name = "textBoxKP";
            this.textBoxKP.Size = new System.Drawing.Size(73, 26);
            this.textBoxKP.TabIndex = 5;
            // 
            // textBoxTI
            // 
            this.textBoxTI.Location = new System.Drawing.Point(107, 114);
            this.textBoxTI.Name = "textBoxTI";
            this.textBoxTI.Size = new System.Drawing.Size(73, 26);
            this.textBoxTI.TabIndex = 6;
            // 
            // textBoxTD
            // 
            this.textBoxTD.Location = new System.Drawing.Point(107, 150);
            this.textBoxTD.Name = "textBoxTD";
            this.textBoxTD.Size = new System.Drawing.Size(73, 26);
            this.textBoxTD.TabIndex = 7;
            // 
            // labelKP
            // 
            this.labelKP.AutoSize = true;
            this.labelKP.Location = new System.Drawing.Point(6, 77);
            this.labelKP.Name = "labelKP";
            this.labelKP.Size = new System.Drawing.Size(33, 20);
            this.labelKP.TabIndex = 1;
            this.labelKP.Text = "KP:";
            // 
            // labelTI
            // 
            this.labelTI.AutoSize = true;
            this.labelTI.Location = new System.Drawing.Point(4, 118);
            this.labelTI.Name = "labelTI";
            this.labelTI.Size = new System.Drawing.Size(45, 20);
            this.labelTI.TabIndex = 2;
            this.labelTI.Text = "TI(s):";
            // 
            // labelTD
            // 
            this.labelTD.AutoSize = true;
            this.labelTD.Location = new System.Drawing.Point(4, 153);
            this.labelTD.Name = "labelTD";
            this.labelTD.Size = new System.Drawing.Size(52, 20);
            this.labelTD.TabIndex = 3;
            this.labelTD.Text = "TD(s):";
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(309, 80);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(73, 508);
            this.richTextBox1.TabIndex = 4;
            this.richTextBox1.Text = "";
            // 
            // zgc
            // 
            this.zgc.Location = new System.Drawing.Point(389, 57);
            this.zgc.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.zgc.Name = "zgc";
            this.zgc.ScrollGrace = 0D;
            this.zgc.ScrollMaxX = 0D;
            this.zgc.ScrollMaxY = 0D;
            this.zgc.ScrollMaxY2 = 0D;
            this.zgc.ScrollMinX = 0D;
            this.zgc.ScrollMinY = 0D;
            this.zgc.ScrollMinY2 = 0D;
            this.zgc.Size = new System.Drawing.Size(754, 572);
            this.zgc.TabIndex = 6;
            // 
            // buttonClearData
            // 
            this.buttonClearData.Location = new System.Drawing.Point(309, 594);
            this.buttonClearData.Name = "buttonClearData";
            this.buttonClearData.Size = new System.Drawing.Size(73, 34);
            this.buttonClearData.TabIndex = 7;
            this.buttonClearData.Text = "Clear";
            this.buttonClearData.UseVisualStyleBackColor = true;
            this.buttonClearData.Click += new System.EventHandler(this.buttonClearData_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(315, 57);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(56, 20);
            this.label1.TabIndex = 8;
            this.label1.Text = "Speed";
            // 
            // buttonDirection
            // 
            this.buttonDirection.Location = new System.Drawing.Point(69, 226);
            this.buttonDirection.Name = "buttonDirection";
            this.buttonDirection.Size = new System.Drawing.Size(147, 33);
            this.buttonDirection.TabIndex = 11;
            this.buttonDirection.Text = "Change direction";
            this.buttonDirection.UseVisualStyleBackColor = true;
            this.buttonDirection.Click += new System.EventHandler(this.buttonDirection_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(144F, 144F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Dpi;
            this.ClientSize = new System.Drawing.Size(1149, 640);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonClearData);
            this.Controls.Add(this.zgc);
            this.Controls.Add(this.richTextBox1);
            this.Controls.Add(this.groupBoxControl);
            this.Controls.Add(this.groupBoxCOMCfg);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "Motor Control";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.MainForm_FormClosed);
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.groupBoxCOMCfg.ResumeLayout(false);
            this.groupBoxCOMCfg.PerformLayout();
            this.groupBoxControl.ResumeLayout(false);
            this.groupBoxControl.PerformLayout();
            this.groupBoxManual.ResumeLayout(false);
            this.groupBoxManual.PerformLayout();
            this.groupBoxPID.ResumeLayout(false);
            this.groupBoxPID.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Button buttonOpenPort;
        private System.Windows.Forms.GroupBox groupBoxCOMCfg;
        private System.Windows.Forms.Label labelCOMName;
        private System.Windows.Forms.Label labelCOMBaudRate;
        private System.Windows.Forms.TextBox textBoxBaudrate;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.GroupBox groupBoxControl;
        private System.Windows.Forms.TextBox textBoxTD;
        private System.Windows.Forms.TextBox textBoxTI;
        private System.Windows.Forms.TextBox textBoxKP;
        private System.Windows.Forms.TextBox textBoxSP;
        private System.Windows.Forms.Label labelTD;
        private System.Windows.Forms.Label labelTI;
        private System.Windows.Forms.Label labelKP;
        private System.Windows.Forms.Label labelSetpoint;
        private System.Windows.Forms.CheckBox checkBoxManual;
        private System.Windows.Forms.GroupBox groupBoxPID;
        private System.Windows.Forms.GroupBox groupBoxManual;
        private System.Windows.Forms.Button buttonSetPWM;
        private System.Windows.Forms.TextBox textBoxPWMWidth;
        private System.Windows.Forms.Label labelPWMPulse;
        private System.Windows.Forms.Button buttonSetTD;
        private System.Windows.Forms.Button buttonSetTI;
        private System.Windows.Forms.Button buttonSetKP;
        private System.Windows.Forms.Button buttonSetSP;
        private System.Windows.Forms.ListBox listBoxPortName;
        private System.Windows.Forms.Button buttonReceive;
        private System.Windows.Forms.RichTextBox richTextBox1;
        private ZedGraph.ZedGraphControl zgc;
        private System.Windows.Forms.Button buttonClearData;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.Button buttonGetParam;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button buttonDirection;
    }
}

