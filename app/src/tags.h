#pragma once

#include <QString>
#include <QVariant>

#include "../../../taglib/taglib/tag.h"
#include <../../taglib/fileref.h>
#include <toolkit/tpropertymap.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>

class Tags {
    TagLib::String name, artist, title, album, genre, fullPath;
    int year, trackNumber;
    TagLib::FileRef f;

    QVariant check(TagLib::String tag) const {
        if (tag.isEmpty()) {
            return "None";
        }
        return tag.toCString();
    }

    void setAllTagsView(TagLib::String str) {
        artist = str;
        title = str;
        album = str;
        genre = str;
        year = 0;
        trackNumber = 0;
    }
public:
    Tags(std::string& file, std::string& path, bool read) {
        name = file;
        fullPath = path;
        if (read) {
            f = TagLib::FileRef(path.c_str());

            if (!f.isNull() && f.tag()) {
                TagLib::Tag *tag = f.tag();

                artist = tag->artist();
                title = tag->title();
                album = tag->album();
                genre = tag->genre();
                year = tag->year();
                trackNumber = tag->track();
            }
        }
        else {
            setAllTagsView("You do not have permissions to read this file.");
        }
    }

    QVariant getTag(int column) const {
        std::vector<QVariant (Tags::*)(void) const> hash {
            &Tags::getName,
            &Tags::getArtist,
            &Tags::getTitle,
            &Tags::getAlbum,
            &Tags::getGenre,
            &Tags::getYear,
            &Tags::getTrack,
            &Tags::getPath
        };
        QVariant res = std::invoke(hash[column], this);

        return res;
    }

    void setTag(int column, const QVariant& value) {
        std::vector<void (Tags::*)(const QVariant& value)> hash {
            &Tags::setName,
            &Tags::setArtist,
            &Tags::setTitle,
            &Tags::setAlbum,
            &Tags::setGenre,
            &Tags::setYear,
            &Tags::setTrack,
            &Tags::setPath
        };

        if (!f.isNull() && f.tag()) {
            (this->*hash[column])(value);
            f.file()->save();
        }
    }

    QVariant getName() const {
        return check(name);
    }

    QVariant getArtist() const {
        return check(artist);
    }

    QVariant getTitle() const {
        return check(title);
    }

    QVariant getAlbum() const {
        return check(album);
    }

    QVariant getGenre() const {
        return check(genre);
    }

    QVariant getPath() const {
        return check(fullPath);
    }

    QVariant getYear() const {
        return year;
    }

    QVariant getTrack() const {
        return trackNumber;
    }

    void setName(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());

        name = newValue;
    }

    void setArtist(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());
        TagLib::Tag *tag = f.tag();

        artist = newValue;
        tag->setArtist(newValue);
    }

    void setTitle(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());
        TagLib::Tag *tag = f.tag();

        title = newValue;
        tag->setTitle(newValue);
    }

    void setAlbum(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());
        TagLib::Tag *tag = f.tag();

        album = newValue;
        tag->setAlbum(newValue);
    }

    void setGenre(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());
        TagLib::Tag *tag = f.tag();

        genre = newValue;
        tag->setGenre(newValue);
    }

    void setYear(const QVariant& value) {
        int newValue(value.toInt());
        TagLib::Tag *tag = f.tag();

        year = newValue;
        tag->setYear(newValue);
    }

    void setTrack(const QVariant& value) {
        int newValue(value.toInt());
        TagLib::Tag *tag = f.tag();

        trackNumber = newValue;
        tag->setTrack(newValue);
    }

    void setPath(const QVariant& value) {
        TagLib::String newValue(value.toString().toStdString());

        fullPath = newValue;
    }
};
