#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"

#include <memory>
#include <string>

using namespace ftxui;


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
  } else {
    color = Color::Red;
    switchText = "Off";
  }
}

 
int main() {
  std::string switchText = "Off";
  bool switchState = false;
  Color switchColor = Color::Red;

  auto lag_button = Button("TOGGLE SWITCH", [&] { toggleSwitch(switchState, switchColor, switchText); }, Style());

  auto component = Renderer(lag_button, [&] {
    return vbox({
      text("SWITCHEROO!") | bold | center,
      separator(),
      text("Switch: " + switchText) | center | color(switchColor),
      separator(),
      lag_button->Render() | center,
    }) | border;
  });

  auto screen = ScreenInteractive::FitComponent();
  screen.Loop(component);

  return 0;
}
