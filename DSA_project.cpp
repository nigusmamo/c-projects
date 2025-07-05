#include <iostream>
#include <string>
using namespace std;

struct Song {
    string title;
    string artist;
    Song* prev;
    Song* next;
};

class MusicPlayer {
private:
    Song* head;
    Song* tail;
    int songCount;
    
public:
    MusicPlayer() : head(nullptr), tail(nullptr), songCount(0) {
        
        addSong("Kochegn", "Aster");
        addSong("Checheho", "Aster");
        addSong("Chewa", "Aster");
        addSong("Nafkote", "Aster");
        addSong("Yewedede", "Aster");
    }
    
    ~MusicPlayer() {
        Song* current = head;
        while (current != nullptr) {
            Song* next = current->next;
            delete current;
            current = next;
        }
    }
    
    void addSong(string title, string artist, int position = -1) {
        if (position == -1) { 
            position = songCount + 1;
        }
        
        if (position < 1 || position > songCount + 1) {
            cout << "Invalid position! Current playlist has " << songCount << " songs.\n";
            return;
        }
        
        Song* newSong = new Song{title, artist, nullptr, nullptr};
        
        if (songCount == 0) { 
            head = tail = newSong;
        } else if (position == 1) { 
            newSong->next = head;
            head->prev = newSong;
            head = newSong;
        } else if (position == songCount + 1) { 
            tail->next = newSong;
            newSong->prev = tail;
            tail = newSong;
        } else { 
            Song* current = head;
            for (int i = 1; i < position - 1; ++i) {
                current = current->next;
            }
            newSong->next = current->next;
            newSong->prev = current;
            current->next->prev = newSong;
            current->next = newSong;
        }
        songCount++;
        cout << "\n♪ Added '" << title << "' by " << artist << " at position " << position << endl;
    }
    
    void removeSong(int position) {
        if (position < 1 || position > songCount) {
            cout << "Invalid position! Current playlist has " << songCount << " songs.\n";
            return;
        }
        
        Song* toRemove;
        
        if (songCount == 1) { 
            toRemove = head;
            head = tail = nullptr;
        } else if (position == 1) {
            toRemove = head;
            head = head->next;
            head->prev = nullptr;
        } else if (position == songCount) { 
            toRemove = tail;
            tail = tail->prev;
            tail->next = nullptr;
        } else { 
            Song* current = head;
            for (int i = 1; i < position; ++i) {
                current = current->next;
            }
            toRemove = current;
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }
        
        cout << "\n♫ Removed '" << toRemove->title << "' by " << toRemove->artist << " from position " << position << endl;
        delete toRemove;
        songCount--;
    }
    
    void displayPlaylist() {
        if (songCount == 0) {
            cout << "\nPlaylist is empty!\n";
            return;
        }
        
        cout << "\n======= Your Playlist (" << songCount << " songs) =======\n";
        Song* current = head;
        int pos = 1;
        while (current != nullptr) {
            cout << pos << ". " << current->title << " - " << current->artist;
            if (pos == 1) cout << " [Currently Playing]";
            cout << endl;
            current = current->next;
            pos++;
        }
        cout << "===============================\n";
    }
    
    int getSongCount() {
        return songCount;
    }
};

int main() {
    MusicPlayer player;
    
    cout << "=== Aster's Music Player ===" << endl;
    cout << "Starting with Aster's songs:\n";
    player.displayPlaylist();
    
    while(true) {
        cout << "\nMenu Options:\n";
        cout << "1. Add a new song\n";
        cout << "2. Remove a song\n";
        cout << "3. Show playlist\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1: {
                string title, artist;
                int position;
                
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter artist name: ";
                getline(cin, artist);
                cout << "Enter position (1-" << player.getSongCount()+1 << "): ";
                cin >> position;
                
                player.addSong(title, artist, position);
                break;
            }
            case 2: {
                if (player.getSongCount() == 0) {
                    cout << "Playlist is empty!\n";
                    break;
                }
                int position;
                player.displayPlaylist();
                cout << "Enter position to remove (1-" << player.getSongCount() << "): ";
                cin >> position;
                player.removeSong(position);
                break;
            }
            case 3:
                player.displayPlaylist();
                break;
            case 4:
                cout << "\nThanks for using Aster's Music Player!\n";
                return 0;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    }
}