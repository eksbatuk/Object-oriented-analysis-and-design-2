package org.example;

import java.sql.*;
import java.util.ArrayList;
import java.util.List;

public class MusicRepository {
    private final IdentityMap identityMap = new IdentityMap();

    private Connection getConnection() throws SQLException {
        return DriverManager.getConnection("jdbc:postgresql://localhost:5432/OOAP2.4", "postgres", "kto prochital tot sdohnet");
    }

    public void clearSession() {
        identityMap.clear();
    }

    public void resetAllTrackStatuses() {
        String sql = "UPDATE tracks SET is_added = FALSE";
        try (Connection conn = getConnection();
             Statement stmt = conn.createStatement()) {
            stmt.executeUpdate(sql);
        } catch (SQLException e) {e.printStackTrace();}
    }

    public void updateTrackStatus(int trackId, boolean isAdded) {
        String sql = "UPDATE tracks SET is_added = ? WHERE id = ?";
        try (Connection conn = getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setBoolean(1, isAdded);
            pstmt.setInt(2, trackId);
            pstmt.executeUpdate();
        } catch (SQLException e) {e.printStackTrace();}
    }

    public List<Artist> getAllArtists() {
        List<Artist> artists = new ArrayList<>();
        String sql = "SELECT id, name FROM artists ORDER BY id";
        try (Connection conn = getConnection();
             Statement stmt = conn.createStatement();
             ResultSet rs = stmt.executeQuery(sql)) {
            while (rs.next()) {
                artists.add(new Artist(rs.getInt("id"), rs.getString("name")));
            }
        } catch (SQLException e) {e.printStackTrace();}
        return artists;
    }

    public List<Album> getAlbumsByArtistId(int artistId) {
        List<Album> albums = new ArrayList<>();
        String sql = "SELECT a.id, a.title, a.cover_path FROM albums a " + "JOIN artist_albums aa ON a.id = aa.album_id " + "WHERE aa.artist_id = ? ORDER BY a.id";

        try (Connection conn = getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {

            pstmt.setInt(1, artistId);
            try (ResultSet rs = pstmt.executeQuery()) {
                while (rs.next()) {
                    albums.add(new Album(
                            rs.getInt("id"),
                            rs.getString("title"),
                            rs.getString("cover_path"),
                            artistId
                    ));
                }
            }
        } catch (SQLException e) {e.printStackTrace();}
        return albums;
    }

    public Track getTrackById(int trackId) {
        Track track = identityMap.getTrack(trackId);
        if (track != null) {
            return track;
        }

        String sql = "SELECT t.id, t.title, t.is_added, a.cover_path FROM tracks t " + "JOIN album_tracks at ON t.id = at.track_id " + "JOIN albums a ON a.id = at.album_id " + "WHERE t.id = ? LIMIT 1";

        try (Connection conn = getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setInt(1, trackId);
            try (ResultSet rs = pstmt.executeQuery()) {
                if (rs.next()) {
                    track = new Track(rs.getInt("id"), rs.getString("title"), rs.getBoolean("is_added"));
                    track.setCoverPath(rs.getString("cover_path"));
                    identityMap.addTrack(track);
                    return track;
                }
            }
        } catch (SQLException e) {e.printStackTrace();}
        return null;
    }

    public List<Track> getTracksByAlbumId(int albumId) {
        List<Track> tracks = new ArrayList<>();
        String sql = "SELECT track_id FROM album_tracks WHERE album_id = ?";

        try (Connection conn = getConnection();
             PreparedStatement pstmt = conn.prepareStatement(sql)) {
            pstmt.setInt(1, albumId);

            try (ResultSet rs = pstmt.executeQuery()) {
                while (rs.next()) {
                    int trackId = rs.getInt("track_id");
                    Track track = getTrackById(trackId);
                    if (track != null) {
                        tracks.add(track);
                    }
                }
            }
        } catch (SQLException e) {e.printStackTrace();}
        return tracks;
    }
}