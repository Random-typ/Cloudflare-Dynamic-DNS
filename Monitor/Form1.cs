using System.Diagnostics;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json.Nodes;
using System.IO.Compression;
using Microsoft.Win32;

namespace Monitor
{
    public partial class Form1 : Form
    {
        public const string ConfigFile = "CloudflareDDNSConfig.json";

        public async void UpdateApp()
        {
            
            if (!checkBox3.Checked)
            {// autoupdate is disabled
                return;
            }
            double InstalledVersion = 1.3;

            string versionsURL = "https://raw.githubusercontent.com/Random-typ/Cloudflare-Dynamic-DNS/master/Monitor/versions.json?";

            if (File.Exists("update.bat"))
            {
                File.Delete("update.bat");
            }

            if (File.Exists("update"))
            {
                File.Delete("update");
            }

            HttpClient httpClient = new HttpClient();

            string content;
            try
            {
                content = await httpClient.GetStringAsync(versionsURL);
            }
            catch (Exception)
            {
                return;
            }
            
            if (content == null)
            {
                return;
            }

            JsonNode json = JsonObject.Parse(content);
            double latest = Convert.ToDouble(json["latest"].ToString());
            if (latest == InstalledVersion)
            {// newest version is already installed
                return;
            }

            DialogResult dr = MessageBox.Show("An update is available. Would you like to update the application now?", "Update Available", MessageBoxButtons.OKCancel);
            if (DialogResult.OK != dr)
            {
                return;
            }

            foreach (var item in json["versions"].AsArray())
            {
                double version = Convert.ToDouble(item["version"].ToString());
                if (version > InstalledVersion && !Convert.ToBoolean(item["skipable"].ToString()) || version == latest)
                {
                    try
                    {
                        Directory.CreateDirectory("update");
                        WebClient web = new WebClient();
                        web.Headers.Add("User-Agent", "Cloudflare DDNS Update");
                        web.DownloadFile(new Uri(item["update"].ToString()), "Update.zip");
                        ZipFile.ExtractToDirectory("Update.zip", "/update");

                        byte[] updatebat = Encoding.ASCII.GetBytes("XCOPY /K /Y \"update\" \"\"\npause\nMonitor.exe");
                        File.WriteAllBytes("update.bat", updatebat);
                        Process.Start("update.bat");
                        this.Close();
                        return;
                    }
                    catch (Exception)
                    {
                        continue;
                    }
                }
            }
            MessageBox.Show("Application cannot be updated. Try again later.");
        }

        public void WriteOut(string _message)
        {
            richTextBox1.Text += _message + "\n";
        }

        public void updateAutostart()
        {
            RegistryKey rk = Registry.CurrentUser.OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", true);

            if (rk == null)
            {
                MessageBox.Show("Couldn't open user registery. Try starting application as administrator.", "Error");

            }
            else
            {

                if (checkBox1.Checked)
                    rk.SetValue("CloudflareDynamicDNS", "\"" + Directory.GetCurrentDirectory() + "/Cloudflare Dynamic DNS.exe" + "\"");
                else
                    if (rk.GetValue("CloudflareDynamicDNS") != null)
                {
                    rk.DeleteValue("CloudflareDynamicDNS", false);
                }
                rk.Close();
            }
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
                string rawconfig = File.ReadAllText(ConfigFile);
                if (rawconfig == null)
                {
                    WriteOut("Failed to load config.");
                    return;
                }
                JsonNode configjson = JsonObject.Parse(rawconfig);
                if (configjson == null)
                {
                    WriteOut("Failed to load config.");
                    return;
                }
                textBox4.Text = configjson["token"]?.ToString();
                
                checkBox1.Checked = Convert.ToBoolean(configjson["autostart"]?.ToString());
                checkBox3.Checked = Convert.ToBoolean(configjson["autoupdate"]?.ToString());
                checkBox2.Checked = Convert.ToBoolean(configjson["IPv6"]?.ToString());
                


                HttpWebRequest request = (HttpWebRequest)WebRequest.Create("https://api.cloudflare.com/client/v4/zones");
                request.Headers.Add("Authorization", "Bearer " + textBox4.Text);
                request.AutomaticDecompression = DecompressionMethods.GZip | DecompressionMethods.Deflate;

                using (HttpWebResponse response = (HttpWebResponse)request.GetResponse())
                using (Stream stream = response.GetResponseStream())
                using (StreamReader reader = new StreamReader(stream))
                {
                    JsonNode json = JsonObject.Parse(reader.ReadToEnd());
                    if (json == null)
                    {
                        WriteOut("Failed to load config.");
                        return;
                    }
                    foreach (var zone in json["result"].AsArray())
                    {
                        string id = zone["id"].ToString();
                        bool ischecked = false;
                        foreach (var allowedzone in configjson["zones"].AsArray())
                        {
                            if (allowedzone.ToString() == id)
                            {
                                ischecked = true;
                                AddZone(zone["name"].ToString(), id, true);
                                break;
                            }
                        }
                        if (!ischecked)
                        {
                            AddZone(zone["name"].ToString(), id, false);
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
            string json = 
                "{\"token\": \"" + textBox4.Text + "\"," +
                "\"IPv6\": \"" + checkBox2.Checked.ToString() + "\"," +
                "\"autoupdate\": \"" + checkBox3.Checked.ToString() + "\"," +
                "\"autostart\": \"" + checkBox1.Checked.ToString() + "\", " +
                "\"asService\": \"" + radioButton2.Checked.ToString() + "\", " +
                "\"serviceInterval\": \"" + numericUpDown1.Value.ToString() + "\", " +
                "\"zones\":[";
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
            try
            {
                Process.Start("https://dash.cloudflare.com/profile/api-tokens");
            }
            catch
            {
                // hack because of this: https://github.com/dotnet/corefx/issues/10361
                if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                {
                    Process.Start(new ProcessStartInfo("https://dash.cloudflare.com/profile/api-tokens") { UseShellExecute = true });
                }
                else if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
                {
                    Process.Start("xdg-open", "https://dash.cloudflare.com/profile/api-tokens");
                }
                else if (RuntimeInformation.IsOSPlatform(OSPlatform.OSX))
                {
                    Process.Start("open", "https://dash.cloudflare.com/profile/api-tokens");
                }
                else
                {
                    throw;
                }
            }
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
            UpdateApp();
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
            // autostart
            updateAutostart();
        }

        private void button4_Click(object sender, EventArgs e)
        {
            WriteConfig();

            // autostart
            updateAutostart();

            // service
            if (radioButton2.Checked)
            {


            }


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
            WriteConfig();
            LoadConfig();
        }

        private void button6_Click(object sender, EventArgs e)
        {
            if (!Directory.Exists(Directory.GetCurrentDirectory() + "\\logs"))
            {
                Directory.CreateDirectory(Directory.GetCurrentDirectory() + "\\logs");
            }
            Process.Start("explorer.exe", Directory.GetCurrentDirectory() + "\\logs");
        }

        private void label4_Click(object sender, EventArgs e)
        {
            if (label4.Text == "show")
            {
                label4.Text = "hide";
                textBox2.UseSystemPasswordChar = false;
            }
            else if (label4.Text == "hide")
            {
                label4.Text = "show";
                textBox2.UseSystemPasswordChar = true;
            }
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            numericUpDown1.Enabled = radioButton2.Checked;


        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {

        }
    }
}