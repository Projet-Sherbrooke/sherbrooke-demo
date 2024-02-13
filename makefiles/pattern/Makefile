all: hello.html bye.html

.PHONY: clean
clean:
	-rm *.html

hello.html: hello.md
bye.html:   bye.md

%.html: %.md ;
	markdown $< > $@

