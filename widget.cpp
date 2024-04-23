/*
Copyright (C) 2011 Georgia Institute of Technology

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include <QTimer>

#include "widget.hpp"

noise_generator::Plugin::Plugin(Event::Manager* ev_manager)
    : Widgets::Plugin(ev_manager, std::string(noise_generator::MODULE_NAME))
{
}

noise_generator::Panel::Panel(QMainWindow* main_window,
                              Event::Manager* ev_manager)
    : Widgets::Panel(
        std::string(noise_generator::MODULE_NAME), main_window, ev_manager)
{
  setWhatsThis(
      "<p><b>Noise Generator:</b></p><p>Generates Gaussian white box noise.");
  createGUI(get_default_vars(), {});  // this is required to create the GUI
  QTimer::singleShot(0, this, SLOT(resizeMe()));
}

noise_generator::Component::Component(Widgets::Plugin* hplugin)
    : Widgets::Component(hplugin,
                         std::string(noise_generator::MODULE_NAME),
                         noise_generator::get_default_channels(),
                         noise_generator::get_default_vars())
    , mean(0.0)
    , variance(1.0)
{
}

void noise_generator::Component::execute()
{
  // This is the real-time function that will be called
  switch (this->getState()) {
    case RT::State::EXEC:
      writeoutput(0, whitenoisewave.get() + mean);
      break;
    case RT::State::INIT:
      mean = getValue<double>(PARAMETER::MEAN);
      variance = getValue<double>(PARAMETER::VARIANCE);
      initStimulus();
      setState(RT::State::EXEC);
    case RT::State::MODIFY:
      mean = getValue<double>(PARAMETER::MEAN);
      variance = getValue<double>(PARAMETER::VARIANCE);
      initStimulus();
      setState(RT::State::PAUSE);
      break;
    case RT::State::PERIOD:
      initStimulus();
      setState(RT::State::EXEC);
      break;
    case RT::State::PAUSE:
      break;
    case RT::State::UNPAUSE:
      setState(RT::State::EXEC);
      break;
    default:
      break;
  }
}

void noise_generator::Component::initParameters()
{
  mean = 0.0;
  variance = 1.0;
}

void noise_generator::Component::initStimulus()
{
  whitenoisewave.clear();
  whitenoisewave.setVariance(variance);
}

///////// DO NOT MODIFY BELOW //////////
// The exception is if your plugin is not going to need real-time functionality.
// For this case just replace the craeteRTXIComponent return type to nullptr.
// RTXI will automatically handle that case and won't attach a component to the
// real time thread for your plugin.

std::unique_ptr<Widgets::Plugin> createRTXIPlugin(Event::Manager* ev_manager)
{
  return std::make_unique<noise_generator::Plugin>(ev_manager);
}

Widgets::Panel* createRTXIPanel(QMainWindow* main_window,
                                Event::Manager* ev_manager)
{
  return new noise_generator::Panel(main_window, ev_manager);
}

std::unique_ptr<Widgets::Component> createRTXIComponent(
    Widgets::Plugin* host_plugin)
{
  return std::make_unique<noise_generator::Component>(host_plugin);
}

Widgets::FactoryMethods fact;

extern "C"
{
Widgets::FactoryMethods* getFactories()
{
  fact.createPanel = &createRTXIPanel;
  fact.createComponent = &createRTXIComponent;
  fact.createPlugin = &createRTXIPlugin;
  return &fact;
}
};

//////////// END //////////////////////
