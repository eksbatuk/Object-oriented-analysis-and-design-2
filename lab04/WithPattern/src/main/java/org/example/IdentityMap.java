package org.example;

import java.util.HashMap;
import java.util.Map;

public class IdentityMap {
    private final Map<Integer, Track> tracks = new HashMap<>();

    public Track getTrack(int id) {
        return tracks.get(id);
    }

    public void addTrack(Track track) {
        tracks.put(track.getId(), track);
    }

    public void clear() {
        tracks.clear();
    }
}