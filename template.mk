LIBNAME=libIterativeRobot
VERSION=0.0.1

TEMPLATESDIR=$(ROOT)/templates

# Files (like header files) & the custom makefile for properly building
# Also has cpp files for the end-user to edit for developing the robot
TEMPLATEFILES = GNUmakefile src/GNUmakefile include/RobotBase.h include/Robot.h \
	include/subsystems/Subsystem.h include/events/EventListener.h \
	include/events/EventScheduler.h include/events/JoystickButton.h \
	include/commands/Command.h include/commands/CommandGroup.h \
	src/init.cpp src/Robot.cpp src/subsystems/ExampleSubsystem.cpp \
	src/commands/ExampleCommand.cpp include/subsystems/ExampleSubsystem.h \
	include/commands/ExampleCommand.h src/commands/ExampleCommandGroup.cpp \
	include/commands/ExampleCommandGroup.h src/commands/ExampleWaitCommand.cpp \
	include/commands/ExampleWaitCommand.h src/events/JoystickChannel.cpp \
	include/events/JoystickChannel.h
# basename of the source files that should be archived
TEMPLATEOBJS = _bin_RobotBase _bin_opcontrol _bin_auto _bin_subsystems_Subsystem\
 	_bin_events_EventScheduler _bin_events_JoystickButton _bin_commands_Command\
	_bin_commands_CommandGroup _bin_events_JoystickChannel

# files to remove (e.g. ones that are changed/removed for this)
REMOVEFILES = auto.c init.c opcontrol.c Makefile

TEMPLATE=$(TEMPLATESDIR)/$(LIBNAME)-template

.DEFAULT_GOAL: all

clean::
	-rm -rf $(TEMPLATESDIR)

library: clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME).a $(foreach f,$(TEMPLATEOBJS),$(BINDIR)/$(f).o)
	mkdir -p $(TEMPLATE) $(TEMPLATE)/firmware $(addprefix $(TEMPLATE)/, $(dir $(TEMPLATEFILES)))
	cp $(BINDIR)/$(LIBNAME).a $(TEMPLATE)/firmware/$(LIBNAME).a
	$(foreach f,$(TEMPLATEFILES),cp $(f) $(TEMPLATE)/$(f);)
	pros conduct create-template $(LIBNAME) $(VERSION) $(TEMPLATE) --ignore template.pros $(foreach f,$(REMOVEFILES),--ignore $(f)) --upgrade-files firmware/$(LIBNAME).a $(foreach f,$(TEMPLATEFILES),--upgrade-files $(f))
	@echo Need to zip $(TEMPLATE) without the base directory $(notdir $(TEMPLATE))
	cd $(TEMPLATE) && zip -r ../$(notdir $(TEMPLATE)) *


# Because I can't remember the difference (-_-)
template: library
	@echo In the future, make sure to use "make library" instead of "make template"
