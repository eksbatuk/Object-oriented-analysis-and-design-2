package org.example;

import javax.swing.*;
import java.awt.*;
import java.net.URL;

public class CoverListCellRender extends DefaultListCellRenderer {

    @Override
    public Component getListCellRendererComponent(JList<?> list, Object value, int index,
                                                  boolean isSelected, boolean cellHasFocus) {
        // Базовая настройка текста и цвета выделения строки
        JLabel label = (JLabel) super.getListCellRendererComponent(list, value, index, isSelected, cellHasFocus);

        String coverPath = null;

        // Проверяем, что именно сейчас рендерится — Альбом или Трек
        if (value instanceof Album) {
            Album album = (Album) value;
            label.setText(album.getTitle());
            coverPath = album.getCoverPath();
        } else if (value instanceof Track) {
            Track track = (Track) value;
            label.setText(track.getTitle());
            coverPath = track.getCoverPath();
        }

        // Ищем картинку в папке resources/covers
        if (coverPath != null) {
            URL imgUrl = getClass().getResource("/covers/" + coverPath);
            if (imgUrl != null) {
                ImageIcon icon = new ImageIcon(imgUrl);
                // Масштабируем картинку до размера 30x30 пикселей, чтобы списки не разъезжались
                Image scaledImg = icon.getImage().getScaledInstance(30, 30, Image.SCALE_SMOOTH);
                label.setIcon(new ImageIcon(scaledImg));
            } else {
                label.setIcon(null); // Если картинку не нашли, оставляем пустой квадрат
            }
        }

        // Делаем небольшой отступ между картинкой и текстом
        label.setIconTextGap(10);
        return label;
    }
}
