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

#include <noise-generator.h>
#include <math.h>
#include <main_window.h>

extern "C" Plugin::Object *createRTXIPlugin(void) {
	return new NoiseGen();
}

static NoiseGen::variable_t vars[] = {
	{ "Noise Waveform", "Noise Waveform", DefaultGUIModel::OUTPUT, },
	{ "Mean", "Mean", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Variance", "Variance", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
};

static size_t num_vars = sizeof(vars) / sizeof(DefaultGUIModel::variable_t);

NoiseGen::NoiseGen(void) : DefaultGUIModel("Noise Generator", ::vars, ::num_vars) {
	setWhatsThis(
	"<p><b>Noise Generator:</b></p><p>Generates noise of the type specified.</p>");
	initParameters();
	initStimulus();
	DefaultGUIModel::createGUI(vars, num_vars); // this is required to create the GUI
	customizeGUI();
	update( INIT);
	refresh();
	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

NoiseGen::~NoiseGen(void) {}

void NoiseGen::execute(void) {
	if (mode == WHITEBM) 
		output(0) = whitenoisewave.get() + mean;
}

void NoiseGen::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("Mean", QString::number(mean));
			setParameter("Variance", QString::number(variance));
			updateMode(2);
			break;
		
		case MODIFY:
			mean = getParameter("Mean").toDouble();
			variance = getParameter("Variance").toDouble();
			whitenoisewave.setVariance(variance);
			initStimulus();
			break;
	
		case PERIOD:
			dt = RT::System::getInstance()->getPeriod() * 1e-9; // time in seconds
			initStimulus();
			break;

		case PAUSE:
			output(0) = 0.0;
			break;
		
		case UNPAUSE:
			break;
		
		default:
			break;
	}
}

void NoiseGen::initParameters() {
	mean = 0;
	variance = 1;
	mode = WHITEBM;
	
	dt = RT::System::getInstance()->getPeriod() * 1e-9; // s
	output(0) = 0;
}

void NoiseGen::initStimulus() {
	if (mode == WHITEBM){
		whitenoisewave.clear();
		whitenoisewave.init(variance);
	}
}

void NoiseGen::updateMode(int state) {
	if (state == 2) {
		mode = WHITEBM;
		update( MODIFY );
	}
	else if (state == 0) {
		mode = OU;
		update( MODIFY );
	}
}

void NoiseGen::customizeGUI(void) {
	QGridLayout *customlayout = DefaultGUIModel::getLayout();
	
	QGroupBox *modeBox = new QGroupBox("Noise Type");
	QHBoxLayout *modeBoxLayout = new QHBoxLayout;
	modeBox->setLayout(modeBoxLayout);
	QCheckBox *wnoiseBMButton = new QCheckBox("White (Box-Muller)");
//	QRadioButton *wnoiseBMButton = new QRadioButton("White (Box-Muller)", modeBox);
	modeBoxLayout->addWidget(wnoiseBMButton);
	wnoiseBMButton->setChecked(true);
//	QObject::connect(modeBox, SIGNAL(clicked(int)), this, SLOT(updateMode(int)));
	QObject::connect(wnoiseBMButton, SIGNAL(stateChanged(int)), this, SLOT(updateMode(int)));
	
	// add custom GUI components to layout above default_gui_model components
	customlayout->addWidget(modeBox, 0, 0);
	setLayout(customlayout);
}
