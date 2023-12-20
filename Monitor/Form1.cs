using System.Diagnostics;
using System.Net;
using System.Runtime.InteropServices;
using System.Text;
using System.Text.Json.Nodes;
using System.IO.Compression;
using Microsoft.Win32;
using System.IO;
using System.Windows.Forms;
using System.Text.RegularExpressions;

namespace Monitor
{
    public partial class Form1 : Form
    {
        public const string ConfigFile = "CloudflareDDNSConfig.json";

        public async void UpdateApp()
        {
            double InstalledVersion = 1.4;
            label1.Text = InstalledVersion.ToString();
            // Cleanup
            if (File.Exists(Directory.GetCurrentDirectory() + "/update.bat"))
            {
                File.Delete(Directory.GetCurrentDirectory() + "/update.bat");
            }

            if (File.Exists(Directory.GetCurrentDirectory() + "/Update.zip"))
            {
                File.Delete(Directory.GetCurrentDirectory() + "/Update.zip");
            }

            if (Directory.Exists(Directory.GetCurrentDirectory() + "/update"))
            {
                Directory.Delete(Directory.GetCurrentDirectory() + "/update", true);
            }




            if (!checkBox3.Checked)
            {// autoupdate is disabled
                return;
            }

            // updating
            string versionsURL = "https://raw.githubusercontent.com/Random-typ/Cloudflare-Dynamic-DNS/master/Monitor/versions.json?";

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
            double latest = Convert.ToDouble(json["latest"].ToString(), new System.Globalization.CultureInfo("en-US"));
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
                double version = Convert.ToDouble(item["version"].ToString(), new System.Globalization.CultureInfo("en-US"));
                if (version > InstalledVersion && !Convert.ToBoolean(item["skipable"].ToString()) || version == latest)
                {
                    try
                    {
                        Directory.CreateDirectory("update");
                        WebClient web = new WebClient();
                        web.Headers.Add("User-Agent", "Cloudflare DDNS Update");
                        web.DownloadFile(new Uri(item["update"].ToString()), "Update.zip");
                        ZipFile.ExtractToDirectory("Update.zip", Directory.GetCurrentDirectory() + "/update");

                        byte[] updatebat = Encoding.ASCII.GetBytes("@echo off\n:check\ntasklist /fi \"ImageName eq Monitor.exe\" /fo csv 2>NUL | find /I \"Monitor.exe\">NUL\r\nif \"%ERRORLEVEL%\"==\"0\" goto check\nXCOPY /K /Y \"" + Directory.GetCurrentDirectory() + "/update\" \"" + Directory.GetCurrentDirectory() + "\"\nMonitor.exe");
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
                return;
            }

            if (checkBox1.Checked)
                rk.SetValue("CloudflareDynamicDNS", "\"" + Directory.GetCurrentDirectory() + "/Cloudflare Dynamic DNS.exe" + "\"");
            else
                rk.DeleteValue("CloudflareDynamicDNS", false);
            rk.Close();
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
        // sends request with cloudflare token and returns content as string
        public string sendGet(string _url)
        {
            HttpRequestMessage request = new HttpRequestMessage(HttpMethod.Get, _url);
            request.Headers.Add("Authorization", "Bearer " + textBox4.Text);
            HttpClient client = new HttpClient();
            HttpResponseMessage response = client.Send(request);
            if (!response.IsSuccessStatusCode)
            {
                WriteOut("Failed to send GET request.");
                return "";
            }
            try
            {
                using (StreamReader reader = new StreamReader(response.Content.ReadAsStream()))
                {
                    return reader.ReadToEnd();
                }
            }
            catch (Exception e)
            {
                MessageBox.Show("Cloudflare API token is invalid or local config file is formatted incorrectly.\n\nDetailed error: \n\n" + e.ToString(), "Error");
                WriteOut("Error loading config.");
            }
            return "";
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

                string zones = sendGet("https://api.cloudflare.com/client/v4/zones");
                if (zones == null)
                {
                    WriteOut("Failed to load zones.");
                    return;
                }
                JsonNode json = JsonObject.Parse(zones);
                if (json == null)
                {
                    WriteOut("Failed to load zones.");
                    return;
                }
                foreach (var zone in json["result"].AsArray())
                {
                    string id = zone["id"].ToString();
                    string dnsRecords = sendGet("https://api.cloudflare.com/client/v4/zones/" + id + "/dns_records");
                    if (dnsRecords == null)
                    {
                        WriteOut("Failed to load DNS records.");
                        return;
                    }
                    JsonNode dnsRecordsJson = JsonObject.Parse(dnsRecords);
                    if (dnsRecordsJson == null)
                    {
                        WriteOut("Failed to load DNS records.");
                        return;
                    }

                    bool ischecked = false;
                    foreach (var allowedzone in configjson["zones"].AsArray())
                    {
                        if (allowedzone.ToString() == id)
                        {
                            ischecked = true;
                            AddZone(zone["name"].ToString(), id, dnsRecordsJson["result"], true);
                            break;
                        }
                    }
                    if (!ischecked)
                    {
                        AddZone(zone["name"].ToString(), id, dnsRecordsJson["result"], false);
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
                if (item.GetType() != typeof(Panel))
                {
                    continue;
                }
                Panel panel = (Panel)item;
                if (!((CheckBox)panel.Controls["checkbox"]).Checked)
                {
                    continue;
                }
                json += "{";
                string id = panel.Controls["label"].Text;
                json += "\"id\": \"" + id.Substring("zone id: ".Length) + "\",";
                json += "\"dns_records\": [";

                foreach (DNSItem record in ((CheckedListBox)panel.Controls["DNSList"]).CheckedItems)
                {
                    json += "\"" + record.id + "\",";
                }
                if (json.EndsWith(','))
                {
                    json = json.Remove(json.Length - 1);
                }
                json += "]";
                json += "}";
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

        public void AddZone(string _domain, string _id, JsonNode _dnsRecords, bool _checked)
        {
            Panel panel = new Panel();

            //panel.Location = new System.Drawing.Point(3, 3);
            panel.Name = "panel";
            panel.Size = new System.Drawing.Size(643, 90);
            panel.TabIndex = 0;
            panel.BorderStyle = BorderStyle.FixedSingle;

            Label label = new Label();

            label.AutoSize = true;
            label.Location = new System.Drawing.Point(3, 44);
            label.Name = "label";
            label.Padding = new System.Windows.Forms.Padding(1, 0, 0, 1);
            label.Dock = DockStyle.Bottom;
            label.TabIndex = 2;
            label.Text = "zone id: " + _id;

            CheckBox checkbox = new CheckBox();

            checkbox.AutoSize = true;
            checkbox.Font = new System.Drawing.Font("Segoe UI", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point);
            checkbox.Padding = new System.Windows.Forms.Padding(2, 3, 0, 0);
            checkbox.Dock = DockStyle.Top;
            checkbox.Name = "checkbox";
            checkbox.Size = new System.Drawing.Size(109, 21);
            checkbox.TabIndex = 1;
            checkbox.Text = _domain;
            checkbox.UseVisualStyleBackColor = true;
            checkbox.Checked = _checked;

            Func<string, string, string, string> makeEntry = (string _type, string _name, string _content) =>
            {
                string entry = _type + "                                                                                                    ";/*100 spaces*/

                if (_name.Length > 20)
                {
                    _name = _name.Substring(0, 17) + "...";
                }

                if (_content.Length > 38)
                {
                    _content = _content.Substring(0, 35) + "...";
                }
                entry = entry.Insert(8, _name);
                entry = entry.Insert(30, _content);
                return entry;
            };

            List<DNSItem> items = new List<DNSItem>();
            foreach (var entry in _dnsRecords.AsArray())
            {
                items.Add(new DNSItem
                {
                    DisplayText = makeEntry(entry["type"].ToString(), entry["name"].ToString(), entry["content"].ToString()),
                    id = entry["type"].ToString()
                });
            }
            CheckedListBox checkedListBox = new CheckedListBox();
            checkedListBox.DataSource = items;
            checkedListBox.FormattingEnabled = true;
            checkedListBox.Name = "DNSList";
            checkedListBox.Size = new Size(400, 100);
            checkedListBox.Location = new System.Drawing.Point(panel.Size.Width - checkedListBox.Size.Width - 2, 0);
            checkedListBox.TabIndex = 17;


            panel.Controls.Add(checkedListBox);
            panel.Controls.Add(checkbox);
            panel.Controls.Add(label);
            flowLayoutPanel1.Controls.Add(panel);
        }

        public void ClearZones()
        {
            flowLayoutPanel1.Controls.Clear();
            flowLayoutPanel1.Controls.Add(label14);
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
            //AddZone("test", "a5acf5ed6859954c617df733fef5e6a2", null, false);
            //return;
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
            UpdateApp();
            Invoke(new Action(startup.ActiveForm.Close));
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

        private void panel2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void flowLayoutPanel1_ControlAdded(object sender, ControlEventArgs e)
        {
            label14.Visible = false;
        }

        private void Form1_Shown(object sender, EventArgs e)
        {
            Activate();
        }
    }
    public class DNSItem
    {
        public string DisplayText { get; set; }
        public string id { get; set; }

        public override string ToString()
        {
            return DisplayText;
        }
    }
}