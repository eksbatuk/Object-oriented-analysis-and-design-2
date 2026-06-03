package org.example;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.util.ArrayList;
import java.util.List;

public class MainUI extends JFrame {
    private final MusicRepository repository = new MusicRepository();

    private enum NavigationState { SHOWING_ARTISTS, SHOWING_ALBUMS, SHOWING_TRACKS }
    private NavigationState currentState = NavigationState.SHOWING_ARTISTS;

    private int currentArtistId = -1;

    private final DefaultListModel<Object> leftModel = new DefaultListModel<>();
    private final DefaultListModel<Track> playlistModel = new DefaultListModel<>();

    private final JList<Object> leftUI = new JList<>(leftModel);
    private final JList<Track> playlistUI = new JList<>(playlistModel);

    private final JButton btnBack = new JButton("Назад");

    public MainUI() {
        repository.resetAllTrackStatuses();

        setSize(750, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLayout(new GridLayout(1, 2, 10, 10));

        JPanel leftPanel = new JPanel(new BorderLayout());
        leftPanel.add(new JLabel("Медиатека", SwingConstants.CENTER), BorderLayout.NORTH);
        leftPanel.add(new JScrollPane(leftUI), BorderLayout.CENTER);
        btnBack.setEnabled(false);
        leftPanel.add(btnBack, BorderLayout.SOUTH);
        add(leftPanel);

        JPanel playlistPanel = new JPanel(new BorderLayout());
        playlistPanel.add(new JLabel("Мой Плейлист", SwingConstants.CENTER), BorderLayout.NORTH);
        playlistPanel.add(new JScrollPane(playlistUI), BorderLayout.CENTER);
        add(playlistPanel);

        leftUI.setCellRenderer(new CoverListCellRenderer());
        playlistUI.setCellRenderer(new CoverListCellRenderer());

        loadArtists();

        leftUI.addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                if (e.getClickCount() == 2) {
                    Object selected = leftUI.getSelectedValue();
                    if (selected == null) return;

                    switch (currentState) {
                        case SHOWING_ARTISTS:
                            Artist artist = (Artist) selected;
                            currentArtistId = artist.getId();
                            loadAlbums(currentArtistId);
                            break;

                        case SHOWING_ALBUMS:
                            Album album = (Album) selected;
                            loadTracks(album.getId());
                            break;

                        case SHOWING_TRACKS:
                            Track track = (Track) selected;
                            if (track.isAdded()) {
                                JOptionPane.showMessageDialog(MainUI.this,
                                        "Этот трек уже добавлен в плейлист!", "Внимание", JOptionPane.WARNING_MESSAGE);
                                return;
                            }

                            track.setAdded(true);
                            repository.updateTrackStatus(track.getId(), true);
                            playlistModel.addElement(track);

                            leftUI.repaint();
                            playlistUI.repaint();
                            break;
                    }
                }
            }
        });

        btnBack.addActionListener(e -> {
            if (currentState == NavigationState.SHOWING_TRACKS) {
                loadAlbums(currentArtistId);
            }
            else if (currentState == NavigationState.SHOWING_ALBUMS) {
                loadArtists();
            }
        });
    }

    private void loadArtists() {
        leftModel.clear();
        currentState = NavigationState.SHOWING_ARTISTS;
        btnBack.setEnabled(false);
        btnBack.setText("<- К артистам");

        List<Artist> artists = repository.getAllArtists();
        for (Artist artist : artists) {
            leftModel.addElement(artist);
        }
    }

    private void loadAlbums(int artistId) {
        leftModel.clear();
        currentState = NavigationState.SHOWING_ALBUMS;
        btnBack.setEnabled(true);
        btnBack.setText("<- Назад к списку артистов");

        List<Album> albums = repository.getAlbumsByArtistId(artistId);
        for (Album album : albums) {
            leftModel.addElement(album);
        }
    }

    private void loadTracks(int albumId) {
        leftModel.clear();
        currentState = NavigationState.SHOWING_TRACKS;
        btnBack.setEnabled(true);
        btnBack.setText("<- Назад к альбомам");

        List<Track> tracksInPlaylist = new ArrayList<>();
        for (int i = 0; i < playlistModel.getSize(); i++) {
            tracksInPlaylist.add(playlistModel.getElementAt(i));
        }

        List<Track> tracks = repository.getTracksByAlbumId(albumId, tracksInPlaylist);
        for (Track track : tracks) {
            leftModel.addElement(track);
        }
    }

    public static void main(String[] args) {
        SwingUtilities.invokeLater(() -> new MainUI().setVisible(true));
    }
}