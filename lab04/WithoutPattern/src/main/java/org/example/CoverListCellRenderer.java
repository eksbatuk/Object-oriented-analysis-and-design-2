package org.example;

import javax.swing.*;
import java.awt.*;
import java.io.File;

public class CoverListCellRenderer extends DefaultListCellRenderer {
    @Override
    public Component getListCellRendererComponent(JList<?> list, Object value, int index,
                                                  boolean isSelected, boolean cellHasFocus) {
        JLabel label = (JLabel) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);
        String coverPath = null;

        if (value instanceof Artist) {
            Artist artist = (Artist) value;
            label.setText(artist.getName());
            label.setIcon(null);
        }

        else if (value instanceof Album) {
            Album album = (Album) value;
            label.setText(album.getTitle());
            coverPath = album.getCoverPath();
        }

        else if (value instanceof Track) {
            Track track = (Track) value;
            if (track.isAdded()) {
                label.setText("✔️ " + track.getTitle());
                label.setFont(label.getFont().deriveFont(Font.BOLD));
            } else {
                label.setText(track.getTitle());
            }
            coverPath = track.getCoverPath();
        }

        if (coverPath != null) {
            String fullPath = ".." + File.separator + "resources" + File.separator + coverPath;
            File imgFile = new File(fullPath);
            if (imgFile.exists()) {
                ImageIcon icon = new ImageIcon(imgFile.getAbsolutePath());
                Image scaledImg = icon.getImage().getScaledInstance(30, 30, Image.SCALE_SMOOTH);
                label.setIcon(new ImageIcon(scaledImg));
            } else {
                label.setIcon(null);
            }
        }
        label.setIconTextGap(10);
        return label;
    }
}

