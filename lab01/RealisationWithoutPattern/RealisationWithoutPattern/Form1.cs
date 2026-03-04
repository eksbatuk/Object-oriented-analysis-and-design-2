using System;
using System.Collections.Generic;
using System.Drawing;
using System.Media;
using System.Windows.Forms;

namespace RealisationWithoutPattern
{
    public partial class Form1 : Form
    {
        private PlayerControl webPlayer;
        private PlayerControl desktopPlayer;
        private PlayerControl mobilePlayer;

        public Form1()
        {
            InitializeComponent();
            CreatePlayers();
        }

        private void CreatePlayers()
        {
            webPlayer = new PlayerControl(
                "WebApp",
                new List<Song> {
                    new Song("Гражданская Оборона - Солнце взойдёт", "..\\..\\..\\..\\AudioFiles\\GrajdanskayaOborona_SolnceVzoydet.wav"),
                    new Song("Twenty One Pilots - Clear", "..\\..\\..\\..\\AudioFiles\\TwentyOnePilots_Clear.wav"),
                    new Song("Megadeth - Tornado of Souls", "..\\..\\..\\..\\AudioFiles\\Megadeth_TornadoOfSouls.wav"),
                    new Song("РАЙАН ГОСЛИНГ", "..\\..\\..\\..\\AudioFiles\\Kavinsky_Nightcall.wav"),
                    new Song("Megurine Luka - Aishite ita no ni", "..\\..\\..\\..\\AudioFiles\\MegurineLuka_AishiteItaNoNi.wav")
                },
                new Point(20, 50),
                this
            );

            desktopPlayer = new PlayerControl(
                "DesktopApp",
                new List<Song> {
                    new Song("Гражданская Оборона - Солнце взойдёт", "..\\..\\..\\..\\AudioFiles\\GrajdanskayaOborona_SolnceVzoydet.wav"),
                    new Song("Twenty One Pilots - Clear", "..\\..\\..\\..\\AudioFiles\\TwentyOnePilots_Clear.wav"),
                    new Song("Megadeth - Tornado of Souls", "..\\..\\..\\..\\AudioFiles\\Megadeth_TornadoOfSouls.wav"),
                    new Song("РАЙАН ГОСЛИНГ", "..\\..\\..\\..\\AudioFiles\\Kavinsky_Nightcall.wav"),
                    new Song("Megurine Luka - Aishite ita no ni", "..\\..\\..\\..\\AudioFiles\\MegurineLuka_AishiteItaNoNi.wav")
                },
                new Point(310, 50),
                this
            );

            mobilePlayer = new PlayerControl(
                "MobileApp",
                new List<Song> {
                    new Song("Гражданская Оборона - Солнце взойдёт", "..\\..\\..\\..\\AudioFiles\\GrajdanskayaOborona_SolnceVzoydet.wav"),
                    new Song("Twenty One Pilots - Clear", "..\\..\\..\\..\\AudioFiles\\TwentyOnePilots_Clear.wav"),
                    new Song("Megadeth - Tornado of Souls", "..\\..\\..\\..\\AudioFiles\\Megadeth_TornadoOfSouls.wav"),
                    new Song("РАЙАН ГОСЛИНГ", "..\\..\\..\\..\\AudioFiles\\Kavinsky_Nightcall.wav"),
                    new Song("Megurine Luka - Aishite ita no ni", "..\\..\\..\\..\\AudioFiles\\MegurineLuka_AishiteItaNoNi.wav")
                },
                new Point(600, 50),
                this
            );

            Controls.Add(webPlayer);
            Controls.Add(desktopPlayer);
            Controls.Add(mobilePlayer);
        }

        public void StopAllPlayers()
        {
            webPlayer.StopPlayer();
            desktopPlayer.StopPlayer();
            mobilePlayer.StopPlayer();
        }

        public void UpdateAllPlayers(PlayerControl currentPlayer)
        {
            webPlayer.UpdateStatus(currentPlayer);
            desktopPlayer.UpdateStatus(currentPlayer);
            mobilePlayer.UpdateStatus(currentPlayer);

        }
    }

    public class Song
    {
        public string Title { get; set; }
        public string FilePath { get; set; }

        public Song(string title, string filePath)
        {
            Title = title;
            FilePath = filePath;
        }

        public override string ToString()
        {
            return Title;
        }
    }

    public class PlayerControl : UserControl
    {
        private ListBox playlistBox;
        private Label statusLabel;
        private Button playButton;
        private Button pauseButton;
        private Button stopButton;

        private string playerName;
        private List<Song> playlist;
        private SoundPlayer soundPlayer;
        private Song currentSong;
        private Form1 mainForm;
        private bool isPlaying;

        public PlayerControl(string name, List<Song> songs, Point location, Form1 form)
        {
            playerName = name;
            playlist = songs;
            this.Location = location;
            this.Size = new Size(280, 320);
            this.BorderStyle = BorderStyle.FixedSingle;
            this.BackColor = Color.White;
            mainForm = form; 

            InitializeComponents();
        }

        private void InitializeComponents()
        {
            var titleLabel = new Label
            {
                Text = playerName,
                Location = new Point(10, 10),
                Font = new Font("Arial", 12, FontStyle.Bold),
                Size = new Size(260, 25),
                TextAlign = ContentAlignment.MiddleCenter
            };

            playlistBox = new ListBox
            {
                Location = new Point(10, 40),
                Size = new Size(260, 150),
                Font = new Font("Arial", 10),
                DataSource = playlist
            };

            statusLabel = new Label
            {
                Location = new Point(10, 200),
                Size = new Size(260, 30),
                Font = new Font("Arial", 9, FontStyle.Italic),
                Text = "⏹ Остановлено",
                TextAlign = ContentAlignment.MiddleCenter,
                BackColor = Color.LightGray
            };

            playButton = new Button
            {
                Text = "▶ Play",
                Location = new Point(10, 240),
                Size = new Size(80, 30),
                BackColor = Color.LightGreen
            };

            pauseButton = new Button
            {
                Text = "⏸ Pause",
                Location = new Point(100, 240),
                Size = new Size(80, 30),
                BackColor = Color.Yellow
            };

            stopButton = new Button
            {
                Text = "⏹ Stop",
                Location = new Point(190, 240),
                Size = new Size(80, 30),
                BackColor = Color.LightCoral
            };

            playButton.Click += PlayButton_Click;
            pauseButton.Click += PauseButton_Click;
            stopButton.Click += StopButton_Click;

            Controls.AddRange(new Control[] {
                titleLabel, playlistBox, statusLabel,
                playButton, pauseButton, stopButton
            });
        }

        private void PlayButton_Click(object sender, EventArgs e)
        {
            Song selectedSong = (Song)playlistBox.SelectedItem;
            currentSong = selectedSong;
            mainForm.StopAllPlayers();
            soundPlayer = new SoundPlayer(selectedSong.FilePath);
            soundPlayer.Play();
            isPlaying = true;
            mainForm.UpdateAllPlayers(this);
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            if (soundPlayer != null && currentSong != null)
            {
                if (isPlaying)
                {
                    soundPlayer.Stop();
                    isPlaying = false;
                }
                else
                {
                    soundPlayer.Play();
                    isPlaying = true;
                }
                mainForm.UpdateAllPlayers(this);
            }
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
                StopPlayer();
        }

        public void UpdateStatus(PlayerControl currentPlayer)
        {
            if (currentPlayer.currentSong == null)
            {
                statusLabel.Text = "⏹ Остановлено";
                statusLabel.BackColor = Color.LightGray;
            }
            else if (currentPlayer.isPlaying)
            {
                if (this.playerName == currentPlayer.playerName)
                {
                    statusLabel.Text = $"▶ Играет: {currentSong}";
                    statusLabel.BackColor = Color.LightGreen;
                }
                else
                {
                    statusLabel.Text = $"▶ Играет на {currentPlayer.playerName}: {currentPlayer.currentSong}";
                    statusLabel.BackColor = Color.LightGreen;
                    playlistBox.SelectedIndex = currentPlayer.playlistBox.SelectedIndex;
                }
            }
            else
            {
                if (this.playerName == currentPlayer.playerName)
                {
                    statusLabel.Text = $"⏸ Пауза: {currentSong}";
                    statusLabel.BackColor = Color.Yellow;
                }
                else
                {
                    statusLabel.Text = $"⏸ Пауза на {currentPlayer.playerName}: {currentPlayer.currentSong}";
                    statusLabel.BackColor = Color.Yellow;
                    playlistBox.SelectedIndex = currentPlayer.playlistBox.SelectedIndex;
                }
            }
        }

        public void StopPlayer()
        {
            if (soundPlayer != null)
            {
                soundPlayer.Stop();
                soundPlayer.Dispose();
                soundPlayer = null;
                currentSong = null;
                isPlaying = false;
                mainForm.UpdateAllPlayers(this);
            }
        }
    }
}
