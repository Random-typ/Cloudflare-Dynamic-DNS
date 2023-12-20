namespace Monitor
{
    partial class Form1
    {
        /// <summary>
        ///  Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///  Clean up any resources being used.
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
        ///  Required method for Designer support - do not modify
        ///  the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            panel1 = new Panel();
            label4 = new Label();
            button1 = new Button();
            linkLabel1 = new LinkLabel();
            label3 = new Label();
            label2 = new Label();
            label1 = new Label();
            textBox2 = new TextBox();
            panel2 = new Panel();
            label11 = new Label();
            label5 = new Label();
            checkBox3 = new CheckBox();
            checkBox2 = new CheckBox();
            checkBox1 = new CheckBox();
            numericUpDown1 = new NumericUpDown();
            label10 = new Label();
            label7 = new Label();
            radioButton2 = new RadioButton();
            radioButton1 = new RadioButton();
            button6 = new Button();
            button2 = new Button();
            richTextBox1 = new RichTextBox();
            label13 = new Label();
            label9 = new Label();
            textBox4 = new TextBox();
            flowLayoutPanel1 = new FlowLayoutPanel();
            label14 = new Label();
            label12 = new Label();
            button5 = new Button();
            label8 = new Label();
            button4 = new Button();
            button3 = new Button();
            label6 = new Label();
            timer1 = new System.Windows.Forms.Timer(components);
            panel1.SuspendLayout();
            panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)numericUpDown1).BeginInit();
            flowLayoutPanel1.SuspendLayout();
            SuspendLayout();
            // 
            // panel1
            // 
            panel1.Controls.Add(label4);
            panel1.Controls.Add(button1);
            panel1.Controls.Add(linkLabel1);
            panel1.Controls.Add(label3);
            panel1.Controls.Add(label2);
            panel1.Controls.Add(label1);
            panel1.Controls.Add(textBox2);
            panel1.Location = new Point(1065, 2);
            panel1.Name = "panel1";
            panel1.Size = new Size(720, 462);
            panel1.TabIndex = 0;
            panel1.Visible = false;
            // 
            // label4
            // 
            label4.AutoSize = true;
            label4.Location = new Point(319, 248);
            label4.Name = "label4";
            label4.Size = new Size(35, 15);
            label4.TabIndex = 16;
            label4.Text = "show";
            label4.Click += label4_Click;
            // 
            // button1
            // 
            button1.Location = new Point(637, 428);
            button1.Name = "button1";
            button1.Size = new Size(75, 23);
            button1.TabIndex = 3;
            button1.Text = "Next";
            button1.UseVisualStyleBackColor = true;
            button1.Click += button1_Click;
            // 
            // linkLabel1
            // 
            linkLabel1.AutoSize = true;
            linkLabel1.Font = new Font("Segoe UI", 9.75F, FontStyle.Regular, GraphicsUnit.Point);
            linkLabel1.Location = new Point(133, 123);
            linkLabel1.Name = "linkLabel1";
            linkLabel1.Size = new Size(177, 17);
            linkLabel1.TabIndex = 6;
            linkLabel1.TabStop = true;
            linkLabel1.Text = "create a Cloudflare API token\r\n";
            linkLabel1.LinkClicked += linkLabel1_LinkClicked;
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Font = new Font("Segoe UI", 9.75F, FontStyle.Regular, GraphicsUnit.Point);
            label3.Location = new Point(32, 123);
            label3.Name = "label3";
            label3.Size = new Size(559, 119);
            label3.TabIndex = 4;
            label3.Text = resources.GetString("label3.Text");
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Font = new Font("Segoe UI", 15.75F, FontStyle.Bold, GraphicsUnit.Point);
            label2.Location = new Point(32, 80);
            label2.Name = "label2";
            label2.Size = new Size(225, 30);
            label2.TabIndex = 3;
            label2.Text = "Cloudflare API token:";
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Font = new Font("Segoe UI", 20.25F, FontStyle.Bold, GraphicsUnit.Point);
            label1.Location = new Point(17, 25);
            label1.Name = "label1";
            label1.Size = new Size(214, 37);
            label1.TabIndex = 2;
            label1.Text = "Getting started";
            // 
            // textBox2
            // 
            textBox2.Location = new Point(32, 245);
            textBox2.Name = "textBox2";
            textBox2.Size = new Size(281, 23);
            textBox2.TabIndex = 0;
            textBox2.UseSystemPasswordChar = true;
            // 
            // panel2
            // 
            panel2.Controls.Add(label11);
            panel2.Controls.Add(label5);
            panel2.Controls.Add(checkBox3);
            panel2.Controls.Add(checkBox2);
            panel2.Controls.Add(checkBox1);
            panel2.Controls.Add(numericUpDown1);
            panel2.Controls.Add(label10);
            panel2.Controls.Add(label7);
            panel2.Controls.Add(radioButton2);
            panel2.Controls.Add(radioButton1);
            panel2.Controls.Add(button6);
            panel2.Controls.Add(button2);
            panel2.Controls.Add(richTextBox1);
            panel2.Controls.Add(label13);
            panel2.Controls.Add(label9);
            panel2.Controls.Add(textBox4);
            panel2.Controls.Add(flowLayoutPanel1);
            panel2.Controls.Add(label12);
            panel2.Controls.Add(button5);
            panel2.Controls.Add(label8);
            panel2.Controls.Add(button4);
            panel2.Controls.Add(button3);
            panel2.Controls.Add(label6);
            panel2.Location = new Point(1, 2);
            panel2.Name = "panel2";
            panel2.Size = new Size(1018, 462);
            panel2.TabIndex = 1;
            panel2.Paint += panel2_Paint;
            // 
            // label11
            // 
            label11.AutoSize = true;
            label11.Font = new Font("Segoe UI", 9.75F, FontStyle.Bold, GraphicsUnit.Point);
            label11.Location = new Point(249, 45);
            label11.Name = "label11";
            label11.Size = new Size(245, 17);
            label11.TabIndex = 33;
            label11.Text = "DNS: Type      Name                Content";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new Point(3, 439);
            label5.Name = "label5";
            label5.Size = new Size(28, 15);
            label5.TabIndex = 32;
            label5.Text = "v1.3";
            // 
            // checkBox3
            // 
            checkBox3.AutoSize = true;
            checkBox3.Location = new Point(679, 93);
            checkBox3.Name = "checkBox3";
            checkBox3.Size = new Size(89, 19);
            checkBox3.TabIndex = 31;
            checkBox3.Text = "Autoupdate";
            checkBox3.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            checkBox2.AutoSize = true;
            checkBox2.Location = new Point(904, 93);
            checkBox2.Name = "checkBox2";
            checkBox2.Size = new Size(70, 19);
            checkBox2.TabIndex = 30;
            checkBox2.Text = "Use IPv6";
            checkBox2.UseVisualStyleBackColor = true;
            // 
            // checkBox1
            // 
            checkBox1.AutoSize = true;
            checkBox1.Location = new Point(679, 68);
            checkBox1.Name = "checkBox1";
            checkBox1.Size = new Size(128, 19);
            checkBox1.TabIndex = 29;
            checkBox1.Text = "Start with Windows";
            checkBox1.UseVisualStyleBackColor = true;
            checkBox1.CheckedChanged += checkBox1_CheckedChanged;
            // 
            // numericUpDown1
            // 
            numericUpDown1.Enabled = false;
            numericUpDown1.Location = new Point(676, 27);
            numericUpDown1.Name = "numericUpDown1";
            numericUpDown1.Size = new Size(75, 23);
            numericUpDown1.TabIndex = 28;
            numericUpDown1.Value = new decimal(new int[] { 10, 0, 0, 0 });
            numericUpDown1.Visible = false;
            // 
            // label10
            // 
            label10.AutoSize = true;
            label10.Location = new Point(757, 32);
            label10.Name = "label10";
            label10.Size = new Size(50, 15);
            label10.TabIndex = 27;
            label10.Text = "Minutes";
            label10.Visible = false;
            // 
            // label7
            // 
            label7.AutoSize = true;
            label7.Location = new Point(676, 7);
            label7.Name = "label7";
            label7.Size = new Size(89, 15);
            label7.TabIndex = 26;
            label7.Text = "Service interval:";
            label7.Visible = false;
            // 
            // radioButton2
            // 
            radioButton2.AutoSize = true;
            radioButton2.Location = new Point(813, 32);
            radioButton2.Name = "radioButton2";
            radioButton2.Size = new Size(78, 19);
            radioButton2.TabIndex = 24;
            radioButton2.TabStop = true;
            radioButton2.Text = "As Service";
            radioButton2.UseVisualStyleBackColor = true;
            radioButton2.Visible = false;
            radioButton2.CheckedChanged += radioButton2_CheckedChanged;
            // 
            // radioButton1
            // 
            radioButton1.AutoSize = true;
            radioButton1.Location = new Point(813, 7);
            radioButton1.Name = "radioButton1";
            radioButton1.Size = new Size(74, 19);
            radioButton1.TabIndex = 23;
            radioButton1.TabStop = true;
            radioButton1.Text = "Autostart";
            radioButton1.UseVisualStyleBackColor = true;
            radioButton1.Visible = false;
            radioButton1.CheckedChanged += radioButton1_CheckedChanged;
            // 
            // button6
            // 
            button6.Location = new Point(849, 430);
            button6.Name = "button6";
            button6.Size = new Size(75, 23);
            button6.TabIndex = 21;
            button6.Text = "open logs";
            button6.UseVisualStyleBackColor = true;
            button6.Click += button6_Click;
            // 
            // button2
            // 
            button2.Location = new Point(20, 413);
            button2.Name = "button2";
            button2.Size = new Size(75, 23);
            button2.TabIndex = 19;
            button2.Text = "reload";
            button2.UseVisualStyleBackColor = true;
            button2.Click += button2_Click;
            // 
            // richTextBox1
            // 
            richTextBox1.Location = new Point(676, 237);
            richTextBox1.Name = "richTextBox1";
            richTextBox1.ReadOnly = true;
            richTextBox1.Size = new Size(301, 119);
            richTextBox1.TabIndex = 0;
            richTextBox1.Text = "";
            // 
            // label13
            // 
            label13.AutoSize = true;
            label13.Location = new Point(677, 127);
            label13.Name = "label13";
            label13.Size = new Size(61, 15);
            label13.TabIndex = 18;
            label13.Text = "API token:";
            // 
            // label9
            // 
            label9.AutoSize = true;
            label9.Location = new Point(979, 151);
            label9.Name = "label9";
            label9.Size = new Size(35, 15);
            label9.TabIndex = 15;
            label9.Text = "show";
            label9.Click += label9_Click;
            // 
            // textBox4
            // 
            textBox4.Location = new Point(677, 148);
            textBox4.Name = "textBox4";
            textBox4.Size = new Size(297, 23);
            textBox4.TabIndex = 14;
            textBox4.UseSystemPasswordChar = true;
            // 
            // flowLayoutPanel1
            // 
            flowLayoutPanel1.AutoScroll = true;
            flowLayoutPanel1.BorderStyle = BorderStyle.FixedSingle;
            flowLayoutPanel1.Controls.Add(label14);
            flowLayoutPanel1.Location = new Point(20, 69);
            flowLayoutPanel1.Name = "flowLayoutPanel1";
            flowLayoutPanel1.Size = new Size(650, 338);
            flowLayoutPanel1.TabIndex = 0;
            flowLayoutPanel1.ControlAdded += flowLayoutPanel1_ControlAdded;
            // 
            // label14
            // 
            label14.Location = new Point(3, 0);
            label14.Name = "label14";
            label14.Size = new Size(634, 328);
            label14.TabIndex = 0;
            label14.Text = "no records";
            label14.TextAlign = ContentAlignment.MiddleCenter;
            // 
            // label12
            // 
            label12.AutoSize = true;
            label12.Font = new Font("Segoe UI", 9.75F, FontStyle.Bold, GraphicsUnit.Point);
            label12.Location = new Point(870, 366);
            label12.Name = "label12";
            label12.Size = new Size(104, 17);
            label12.TabIndex = 12;
            label12.Text = "Status: stopped";
            // 
            // button5
            // 
            button5.Enabled = false;
            button5.Location = new Point(789, 364);
            button5.Name = "button5";
            button5.Size = new Size(75, 23);
            button5.TabIndex = 11;
            button5.Text = "stop";
            button5.UseVisualStyleBackColor = true;
            button5.Click += button5_Click;
            // 
            // label8
            // 
            label8.AutoSize = true;
            label8.Location = new Point(676, 219);
            label8.Name = "label8";
            label8.Size = new Size(48, 15);
            label8.TabIndex = 8;
            label8.Text = "Output:";
            // 
            // button4
            // 
            button4.Enabled = false;
            button4.Location = new Point(677, 364);
            button4.Name = "button4";
            button4.Size = new Size(75, 23);
            button4.TabIndex = 6;
            button4.Text = "start";
            button4.UseVisualStyleBackColor = true;
            button4.Click += button4_Click;
            // 
            // button3
            // 
            button3.Location = new Point(930, 430);
            button3.Name = "button3";
            button3.Size = new Size(75, 23);
            button3.TabIndex = 5;
            button3.Text = "save";
            button3.UseVisualStyleBackColor = true;
            button3.Click += button3_Click;
            // 
            // label6
            // 
            label6.AutoSize = true;
            label6.Font = new Font("Segoe UI", 15.75F, FontStyle.Bold, GraphicsUnit.Point);
            label6.Location = new Point(20, 7);
            label6.Name = "label6";
            label6.Size = new Size(334, 30);
            label6.TabIndex = 3;
            label6.Text = "Selected zones and DNS records:";
            // 
            // timer1
            // 
            timer1.Interval = 200;
            timer1.Tick += timer1_Tick;
            // 
            // Form1
            // 
            AutoScaleDimensions = new SizeF(7F, 15F);
            AutoScaleMode = AutoScaleMode.Font;
            ClientSize = new Size(1856, 465);
            Controls.Add(panel1);
            Controls.Add(panel2);
            Name = "Form1";
            ShowIcon = false;
            StartPosition = FormStartPosition.CenterScreen;
            Text = "Cloudflare Dynamic DNS Monitor";
            Load += Form1_Load;
            Shown += Form1_Shown;
            panel1.ResumeLayout(false);
            panel1.PerformLayout();
            panel2.ResumeLayout(false);
            panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)numericUpDown1).EndInit();
            flowLayoutPanel1.ResumeLayout(false);
            ResumeLayout(false);
        }

        #endregion

        private Panel panel1;
        private Label label3;
        private Label label2;
        private Label label1;
        private TextBox textBox2;
        private Panel panel2;
        private LinkLabel linkLabel1;
        private Button button1;
        private Label label8;
        private Button button4;
        private Button button3;
        private Label label6;
        private FlowLayoutPanel flowLayoutPanel1;
        private Label label12;
        private Button button5;
        private Label label13;
        private Label label9;
        private TextBox textBox4;
        private System.Windows.Forms.Timer timer1;
        private RichTextBox richTextBox1;
        private Button button2;
        private Button button6;
        private Label label4;
        private NumericUpDown numericUpDown1;
        private Label label10;
        private Label label7;
        private RadioButton radioButton2;
        private RadioButton radioButton1;
        private CheckBox checkBox1;
        private CheckBox checkBox2;
        private CheckBox checkBox3;
        private Label label5;
        private Label label11;
        private Label label14;
    }
}