LIBNAME=libIterativeRobot
VERSION=0.0.1

TEMPLATESDIR=$(ROOT)/templates

# extra files (like header files)
TEMPLATEFILES = include/RobotBase.h include/subsystems/Subsystem.h include/events/EventListener.h include/events/EventScheduler.h include/events/JoystickButton.h include/commands/Command.h include/commands/CommandGroup.h
# basename of the source files that should be archived
TEMPLATEOBJS = _bin_RobotBase _bin_subsystems_Subsystem _bin_events_EventScheduler _bin_events_JoystickButton _bin_commands_Command _bin_commands_CommandGroup

TEMPLATE=$(TEMPLATESDIR)/$(LIBNAME)-template

clean::
	-rm -rf $(TEMPLATESDIR)

.DEFAULT_GOAL: all

library: clean $(BINDIR) $(SUBDIRS) $(ASMOBJ) $(COBJ) $(CPPOBJ)
	$(MCUPREFIX)ar rvs $(BINDIR)/$(LIBNAME).a $(foreach f,$(TEMPLATEOBJS),$(BINDIR)/$(f).o)
	mkdir -p $(TEMPLATE) $(TEMPLATE)/firmware $(addprefix $(TEMPLATE)/, $(dir $(TEMPLATEFILES)))
	cp $(BINDIR)/$(LIBNAME).a $(TEMPLATE)/firmware/$(LIBNAME).a
	$(foreach f,$(TEMPLATEFILES),cp $(f) $(TEMPLATE)/$(f);)
	pros conduct create-template $(LIBNAME) $(VERSION) $(TEMPLATE) --ignore template.pros --upgrade-files firmware/$(LIBNAME).a $(foreach f,$(TEMPLATEFILES),--upgrade-files $(f))
	@echo Need to zip $(TEMPLATE) without the base directory $(notdir $(TEMPLATE))
	cd $(TEMPLATE) && zip -r ../$(notdir $(TEMPLATE)) *


# Because I can't remember the difference (-_-)
template: library
	@echo In the future, make sure to use "make library" instead of "make template"
