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
#include <QtGui>

extern "C" Plugin::Object *createRTXIPlugin(void) {
	return new NoiseGen();
}

static NoiseGen::variable_t vars[] = {
	{ "Noise Waveform", "Noise Waveform", DefaultGUIModel::OUTPUT, },
	{ "Mean", "Mean", DefaultGUIModel::PARAMETER | DefaultGUIModel::DOUBLE, },
	{ "Variance", "Variance", DefaultGUIModel::PARAMETER
		| DefaultGUIModel::DOUBLE, },
	{ "Time (s)", "Time (s)", DefaultGUIModel::STATE, }, 
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
	printf("Starting NoiseGen Module:\n");

	QTimer::singleShot(0, this, SLOT(resizeMe()));
}

NoiseGen::~NoiseGen(void) {}

void NoiseGen::execute(void) {
	systime = count * dt; // time in seconds
	
	if (mode == WHITEBM) output(0) = whitenoisewave.get() + mean;
	count++; // increment time
}

void NoiseGen::update(DefaultGUIModel::update_flags_t flag) {
	switch (flag) {
		case INIT:
			setParameter("Mean", QString::number(mean));
			setParameter("Variance", QString::number(variance));
			setState("Time (s)", systime);
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
			systime = 0;
			count = 0;
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
	systime = 0;
	count = 0;
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
		printf("Noise generator now set to Gaussian white noise generated using the Box-Muller method\n");
		update( MODIFY);
	}
	else if (state == 0) {
		mode = OU;
		printf("Noise generator now set to OU process\n");
		update( MODIFY );
	}
}

//void NoiseGen::createGUI(DefaultGUIModel::variable_t *var, int size)

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
/*
QScrollView *sv = new QScrollView(this);
sv->setResizePolicy(QScrollView::AutoOneFit);
layout->addWidget(sv);

QWidget *viewport = new QWidget(sv->viewport());
sv->addChild(viewport);
QGridLayout *scrollLayout = new QGridLayout(viewport, 1, 2);

size_t nstate = 0, nparam = 0, nevent = 0, ncomment = 0;
for (size_t i = 0; i < num_vars; i++)
	{
	if (vars[i].flags & (PARAMETER | STATE | EVENT | COMMENT))
		{
		param_t param;
		
		param.label = new QLabel(vars[i].name, viewport);
		scrollLayout->addWidget(param.label, parameter.size(), 0);
		param.edit = new DefaultGUILineEdit(viewport);
		scrollLayout->addWidget(param.edit, parameter.size(), 1);
		
		QToolTip::add(param.label, vars[i].description);
		QToolTip::add(param.edit, vars[i].description);
		
		if (vars[i].flags & PARAMETER)
			{
			if (vars[i].flags & DOUBLE)
				{
				param.edit->setValidator(new QDoubleValidator(param.edit));
				param.type = PARAMETER | DOUBLE;
			}
			else if (vars[i].flags & UINTEGER)
			{
				QIntValidator *validator = new QIntValidator(param.edit);
				param.edit->setValidator(validator);
				validator->setBottom(0);
				param.type = PARAMETER | UINTEGER;
			}
			else if (vars[i].flags & INTEGER)
			{
				param.edit->setValidator(new QIntValidator(param.edit));
				param.type = PARAMETER | INTEGER;
			}
			else
			param.type = PARAMETER;
			param.index = nparam++;
			param.str_value = new QString;
		}
		else if (vars[i].flags & STATE)
		{
			param.edit->setReadOnly(true);
			param.edit->setPaletteForegroundColor(Qt::darkGray);
			param.type = STATE;
			param.index = nstate++;
		}
		else if (vars[i].flags & EVENT)
		{
			param.edit->setReadOnly(true);
			param.type = EVENT;
			param.index = nevent++;
		}
		else if (vars[i].flags & COMMENT)
		{
			param.type = COMMENT;
			param.index = ncomment++;
		}
		
		parameter[vars[i].name] = param;
	}
}

QHBox *hbox1 = new QHBox(this);
pauseButton = new QPushButton("Pause", hbox1);
pauseButton->setToggleButton(true);
QObject::connect(pauseButton,SIGNAL(toggled(bool)),this,SLOT(pause(bool)));
QPushButton *modifyButton = new QPushButton("Modify", hbox1);
QObject::connect(modifyButton,SIGNAL(clicked(void)),this,SLOT(modify(void)));
QPushButton *unloadButton = new QPushButton("Unload", hbox1);
QObject::connect(unloadButton,SIGNAL(clicked(void)),this,SLOT(exit(void)));
layout->addWidget(hbox1);

show();

}
*/
