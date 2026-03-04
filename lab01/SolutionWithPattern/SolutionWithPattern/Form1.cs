using System;
using System.Collections.Generic;
using System.Drawing;
using System.Media;
using System.Windows.Forms;

namespace SolutionWithPattern
{
    public partial class Form1 : Form
    {
        private MainPlayer mainPlayer;
        private PlayerControl webPlayer;
        private PlayerControl desktopPlayer;
        private PlayerControl mobilePlayer;

        public Form1()
        {
            InitializeComponent();
            mainPlayer = MainPlayer.GetInstance();
            CreatePlayers();
        }

        private void CreatePlayers()
        {
            webPlayer = new PlayerControl(
                "WebApp",
                new Point(20, 50),
                mainPlayer
            );

            desktopPlayer = new PlayerControl(
                "DesktopApp",
                new Point(310, 50),
                mainPlayer
            );

            mobilePlayer = new PlayerControl(
                "MobileApp",
                new Point(600, 50),
                mainPlayer
            );

            Controls.Add(webPlayer);
            Controls.Add(desktopPlayer);
            Controls.Add(mobilePlayer);
        }
    }

    public class MainPlayer
    {
        private static MainPlayer instance;
        private List<Song> masterPlaylist;
        private List<PlayerControl> players = new List<PlayerControl>();
        private SoundPlayer soundPlayer;
        private Song currentSong;
        private bool isPlaying;
        private string activePlayer;

        private MainPlayer()
        {
            masterPlaylist = new List<Song> {
                                 new Song("Гражданская Оборона - Солнце взойдёт", "..\\..\\..\\..\\AudioFiles\\GrajdanskayaOborona_SolnceVzoydet.wav"),
                                 new Song("Twenty One Pilots - Clear", "..\\..\\..\\..\\AudioFiles\\TwentyOnePilots_Clear.wav"),
                                 new Song("Megadeth - Tornado of Souls", "..\\..\\..\\..\\AudioFiles\\Megadeth_TornadoOfSouls.wav"),
                                 new Song("РАЙАН ГОСЛИНГ", "..\\..\\..\\..\\AudioFiles\\Kavinsky_Nightcall.wav"),
                                 new Song("Megurine Luka - Aishite ita no ni", "..\\..\\..\\..\\AudioFiles\\MegurineLuka_AishiteItaNoNi.wav")
                                 };

            soundPlayer = new SoundPlayer();
        }

        private void UpdateAllPlayers()
        {
            foreach (var player in players)
            {
                player.UpdateStatus();
            }
        }

        public void RegisterPlayer(PlayerControl player)
        {
            if (!players.Contains(player))
            {
                players.Add(player);
            }
        }

        public void Play(Song song, string playerName)
        {
            soundPlayer.Stop();
            soundPlayer.SoundLocation = song.FilePath;
            soundPlayer.Play();
            currentSong = song;
            isPlaying = true;
            activePlayer = playerName;
            
            UpdateAllPlayers();
        }

        public void Pause()
        {
            if (currentSong != null)
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
                
                UpdateAllPlayers();
            }
        }

        public void Stop()
        {
            soundPlayer.Stop();
            currentSong = null;
            isPlaying = false;
            
            UpdateAllPlayers();
        }

        public Song GetCurrentSong()
        {
            return currentSong;
        }

        public bool IsPlaying()
        {
            return isPlaying && currentSong != null;
        }

        public string GetStatusText(PlayerControl player)
        {
            if (currentSong == null)
                return "⏹ Остановлено";

            if (player.GetPlayerName() == activePlayer)
            {
                if (isPlaying)
                    return $"▶ Играет: {currentSong.Title}";
                else
                    return $"⏸ Пауза: {currentSong.Title}";
            }
            else
            {
                if (isPlaying)
                    return $"▶ Играет на {activePlayer}: {currentSong.Title}";
                else
                    return $"⏸ Пауза на {activePlayer}: {currentSong.Title}";
            }
        }

        public List<Song> GetMasterPlaylist()
        {
            return masterPlaylist;
        }

        public static MainPlayer GetInstance()
        {
            if (instance == null)
            {
                instance = new MainPlayer();
            }
            return instance;
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
        private MainPlayer mainPlayer;

        public PlayerControl(string name, Point location, MainPlayer player)
        {
            playerName = name;
            this.Location = location;
            this.Size = new Size(280, 320);
            this.BorderStyle = BorderStyle.FixedSingle;
            this.BackColor = Color.White;
            mainPlayer = player;
            mainPlayer.RegisterPlayer(this);
            InitializeComponents();
            UpdateStatus();
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
                DataSource = mainPlayer.GetMasterPlaylist()
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
            mainPlayer.Play(selectedSong, playerName);
        }

        private void PauseButton_Click(object sender, EventArgs e)
        {
            mainPlayer.Pause();
        }

        private void StopButton_Click(object sender, EventArgs e)
        {
            mainPlayer.Stop();
        }

        public void UpdateStatus()
        {
            statusLabel.Text = mainPlayer.GetStatusText(this);

            Song currentSong = mainPlayer.GetCurrentSong();
            if (currentSong == null)
            {
                statusLabel.BackColor = Color.LightGray;
            }
            else if (mainPlayer.IsPlaying())
            {
                statusLabel.BackColor = Color.LightGreen;
            }
            else
            {
                statusLabel.BackColor = Color.Yellow;
            }

            if (currentSong != null && playlistBox.Items.Contains(currentSong))
            {
                playlistBox.SelectedItem = currentSong;
            }
        }

        public string GetPlayerName()
        {
            return playerName; 
        }
    }
}