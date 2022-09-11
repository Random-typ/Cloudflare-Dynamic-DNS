using System.Diagnostics;
using System.Net;
using System.Text;
using System.Text.Json.Nodes;


namespace Monitor
{
    public partial class Form1 : Form
    {
        public const string ConfigFile = "CloudflareDDNSConfig.json";

        public void WriteOut(string _message)
        {
            richTextBox1.Text += _message + "\n";
        }

        public void DDNSStatus()
        {
            Process[] proc = Process.GetProcessesByName("Cloudflare Dynamic DNS");
            if (proc.Length == 0)
            {
                button4.Enabled = true;
                button5.Enabled = false;
                label12.Text = "Status: stopped";
            }
            else
            {
                button4.Enabled = false;
                button5.Enabled = true;
                label12.Text = "Status: running";
            }
        }

        public void LoadConfig()
        {
            WriteOut("Loading config...");
            try
            {
                JsonNode configjson = JsonObject.Parse(File.ReadAllText(ConfigFile));
                textBox4.Text = configjson["token"].ToString();

                HttpWebRequest request = (HttpWebRequest)WebRequest.Create("https://api.cloudflare.com/client/v4/zones");
                request.Headers.Add("Authorization", "Bearer " + textBox4.Text);
                request.AutomaticDecompression = DecompressionMethods.GZip | DecompressionMethods.Deflate;

                using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
                using (Stream stream = response.GetResponseStream())
                using (StreamReader reader = new StreamReader(stream))
                {
                    JsonNode json = JsonObject.Parse(reader.ReadToEnd());
                    foreach (var zone in json["result"].AsArray())
                    {
                        string id = zone["id"].ToString();
                        for (int i = 0; i < configjson["zones"].AsArray().Count; i++)
                        {
                            if (configjson["zones"].AsArray()[i].ToString() == id)
                            {
                                AddZone(zone["name"].ToString(), id, true);
                                break;
                            }
                            if (i + 1 == configjson["zones"].AsArray().Count)
                            {
                                AddZone(zone["name"].ToString(), id, false);
                            }
                        }
                    }
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Cloudflare API token is invalid or local config file is formatted incorrectly.\n\nDetailed error: \n\n" + e.ToString(), "Error");
                WriteOut("Error loading config.");
                return;
            }
            WriteOut("Loaded config.");
        }

        public void WriteConfig()
        {
            string json = "{\"token\": \"" + textBox4.Text + "\", \"zones\":[";
            foreach (var item in flowLayoutPanel1.Controls)
            {
                Panel panel = (Panel)item;
                if (!((CheckBox)panel.Controls["checkbox"]).Checked)
                {
                    continue;
                }
                string id = panel.Controls["label"].Text;
                json += "\"" + id.Substring("zone id: ".Length) + "\",";

            }
            if (json.EndsWith(','))
            {
                json = json.Remove(json.Length - 1);
            }
            json += "]}";
            FileStream file = File.Create(ConfigFile);
            file.Write(Encoding.ASCII.GetBytes(json), 0, json.Length);
            file.Close();
            WriteOut("Saved config.");
        }

        public void AddZone(string _domain, string _id, bool _checked)
        {
            Panel panel = new Panel();

            panel.Location = new System.Drawing.Point(3, 3);
            panel.Name = "panel";
            panel.Size = new System.Drawing.Size(319, 67);
            panel.TabIndex = 0;
            panel.BorderStyle = BorderStyle.FixedSingle;

            Label label = new Label();

            label.AutoSize = true;
            label.Location = new System.Drawing.Point(3, 44);
            label.Name = "label";
            label.Size = new System.Drawing.Size(61, 15);
            label.TabIndex = 2;
            label.Text = "zone id: " + _id;

            CheckBox checkbox = new CheckBox();

            checkbox.AutoSize = true;
            checkbox.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            checkbox.Location = new System.Drawing.Point(3, 8);
            checkbox.Name = "checkbox";
            checkbox.Size = new System.Drawing.Size(109, 21);
            checkbox.TabIndex = 1;
            checkbox.Text = _domain;
            checkbox.UseVisualStyleBackColor = true;
            checkbox.Checked = _checked;

            panel.Controls.Add(checkbox);
            panel.Controls.Add(label);
            flowLayoutPanel1.Controls.Add(panel);
        }

        public void ClearZones()
        {
            flowLayoutPanel1.Controls.Clear();
            WriteOut("Cleared zones.");
        }

        public Form1()
        {
            InitializeComponent();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            Process.Start("https://dash.cloudflare.com/profile/api-tokens");
        }

        private void label9_Click(object sender, EventArgs e)
        {
            if (label9.Text == "show")
            {
                label9.Text = "hide";
                textBox4.UseSystemPasswordChar = false;
            }
            else if (label9.Text == "hide")
            {
                label9.Text = "show";
                textBox4.UseSystemPasswordChar = true;
            }
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            ClientSize = new Size(panel2.Size.Width + 20, panel2.Size.Height + 40);
            MaximumSize = ClientSize;
            MinimumSize = ClientSize;

            timer1.Start();

            if (!File.Exists(ConfigFile))
            {
                panel1.Location = panel2.Location;
                panel1.Visible = true;
                return;
            }
            LoadConfig();
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            DDNSStatus();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            if (textBox2.Text.Length == 0)
            {
                return;
            }

            string json = "{\"token\": \"" + textBox2.Text + "\", \"zones\":[]}";
            FileStream file = File.Create(ConfigFile);
            file.Write(Encoding.ASCII.GetBytes(json), 0, json.Length);
            file.Close();
            panel1.Visible = false;
            LoadConfig();
        }


        private void button3_Click(object sender, EventArgs e)
        {
            WriteConfig();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            Process.Start("Cloudflare Dynamic DNS.exe");
            WriteOut("Started Dynamic DNS.");
        }

        private void button5_Click(object sender, EventArgs e)
        {
            foreach (var process in Process.GetProcessesByName("Cloudflare Dynamic DNS"))
            {
                process.Kill();
            }
            WriteOut("Stopped Dynamic DNS.");
        }

        private void button2_Click(object sender, EventArgs e)
        {
            ClearZones();
            LoadConfig();
        }
    }
}