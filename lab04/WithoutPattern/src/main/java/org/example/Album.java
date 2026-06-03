package org.example;

public class Album{
    private final int id;
    private final String title;
    private final String coverPath;

    public Album(int id, String title, String coverPath, int artistId) {
        this.id = id;
        this.title = title;
        this.coverPath = coverPath;
    }

    public int getId() { return id; }
    public String getTitle() { return title; }
    public String getCoverPath() { return coverPath; }

    @Override
    public String toString() { return title; }
}