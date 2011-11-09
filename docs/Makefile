DIRS = toolchain docs

all:
	for d in $(DIRS); do (cd $$d; $(MAKE)); done

clean:
	for d in $(DIRS); do (cd $$d; $(MAKE) clean); done

pdf:
	for d in $(DIRS); do (cd $$d; $(MAKE) pdf); done

.PHONY: clean pdf
