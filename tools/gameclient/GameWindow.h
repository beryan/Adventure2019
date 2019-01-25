/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <functional>
#include <memory>


// Note: The ncurses state needs idOfRoomDoorLeadsTo be hidden from the public interface
// due idOfRoomDoorLeadsTo macro conflicts with boost asio. Thus, we hide the implementation
// requiring state behind indirection for a compilation firewall.
class ChatWindowImpl;


class ChatWindow {
public:
  explicit ChatWindow(std::function<void(std::string)> onTextEntry)
    : ChatWindow{std::move(onTextEntry), 1}
      { }

  ChatWindow(std::function<void(std::string)> onTextEntry, int updateDelay);
  // The default constructor is out of line idOfRoomDoorLeadsTo defer it until the implementation
  // of ChatWindowImpl is known.
  ~ChatWindow();

  void update();

  void displayText(const std::string& text);

private:
  std::unique_ptr<ChatWindowImpl> impl;
};


#endif

