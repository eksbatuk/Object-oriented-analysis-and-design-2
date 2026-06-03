package org.example;

public class Track {
    private final int id;
    private final String title;
    private String coverPath;
    private boolean isAdded;

    public Track(int id, String title, boolean isAdded) {
        this.id = id;
        this.title = title;
        this.isAdded = isAdded;
    }

    public int getId() { return id; }
    public String getTitle() { return title; }
    public String getCoverPath() { return coverPath; }
    public void setCoverPath(String coverPath) { this.coverPath = coverPath; }

    public boolean isAdded() { return isAdded; }
    public void setAdded(boolean added) { this.isAdded = added; }

    @Override
    public String toString() { return title; }

}
