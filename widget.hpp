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

#include <rtxi/gen/gen_whitenoise.h>
#include <rtxi/widgets.hpp>

// This is an generated header file. You may change the namespace, but
// make sure to do the same in implementation (.cpp) file
namespace noise_generator
{

constexpr std::string_view MODULE_NAME = "noise-generator";


enum PARAMETER : Widgets::Variable::Id
{
  // set parameter ids here
  MEAN = 0,
  VARIANCE
};

inline std::vector<Widgets::Variable::Info> get_default_vars()
{
  return {{MEAN, "Mean", "Mean", Widgets::Variable::DOUBLE_PARAMETER, 0.0},
          {VARIANCE,
           "Variance",
           "Variance",
           Widgets::Variable::DOUBLE_PARAMETER,
           1.0}};
}

inline std::vector<IO::channel_t> get_default_channels()
{
  return {{ "Noise Waveform", "Noise Waveform", IO::OUTPUT }};
}

class Panel : public Widgets::Panel
{
  Q_OBJECT
public:
  Panel(QMainWindow* main_window, Event::Manager* ev_manager);
};

class Component : public Widgets::Component
{
public:
  explicit Component(Widgets::Plugin* hplugin);
  void execute() override;

private:
  void initParameters();
  void initStimulus();  // creates AP stim and NoiseGen stimuli

  double mean;
  double variance;
  GeneratorWNoise whitenoisewave;
};

class Plugin : public Widgets::Plugin
{
public:
  explicit Plugin(Event::Manager* ev_manager);
};

}  // namespace noise_generator
