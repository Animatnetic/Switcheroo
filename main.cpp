#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <winsock2.h>
#include <stdio.h>
#include <memory>
#include <string>
#include <thread>
#include <atomic>

#include "include/windivert.h"
#include <windows.h>


using namespace ftxui;

HANDLE g_hFilter = INVALID_HANDLE_VALUE;

void startDroppingPackets() {
  g_hFilter = WinDivertOpen("inbound", WINDIVERT_LAYER_NETWORK, 0, WINDIVERT_FLAG_DROP);

  if (g_hFilter == INVALID_HANDLE_VALUE) {
    MessageBox(
      NULL, 
      std::to_string(GetLastError()).c_str(),
      "SWITCHEROO - Error",
      MB_OK | MB_ICONINFORMATION
    );
  }
}

void stopDroppingPackets() {
  WinDivertClose(g_hFilter);
  g_hFilter = INVALID_HANDLE_VALUE;
}

ButtonOption Style() {
  auto option = ButtonOption::Animated();
  option.transform = [](const EntryState& s) {
    auto element = text(s.label);
    if (s.focused) {
      element |= bold;
    }

    return element | center;
  };

  return option;
}

void toggleSwitch(bool& state, Color& color, std::string& switchText) {
  state = !state;

  if (state == true) {
    color = Color::Green;
    switchText = "On";
    startDroppingPackets();
  } else {
    color = Color::Red;
    switchText = "Off";
    stopDroppingPackets();
  }
}

 
int main() {
  std::string switchText = "Off";
  bool switchState = false;
  Color switchColor = Color::Red;

  auto lag_button = Button("TOGGLE SWITCH", [&] { toggleSwitch(switchState, switchColor, switchText); }, Style());

  auto component = Renderer(lag_button, [&] {
    return vbox({
      text("SWITCHEROO! v1.0") | bold | center,
      separator(),
      text("Switch: " + switchText) | center | color(switchColor),
      separator(),
      lag_button->Render() | center,
    }) | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);
  
  stopDroppingPackets();
  return 0;
}
