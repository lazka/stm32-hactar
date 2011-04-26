NAME=doc

all: $(NAME).html

$(NAME).html: $(NAME).rst style.css
	rst2html --stylesheet=style.css $(NAME).rst > $(NAME).html

pdf: $(NAME).pdf

$(NAME).pdf: $(NAME).rst
	rst2pdf -b1 $(NAME).rst

clean:
	rm $(NAME).html $(NAME).pdf

show:
	xdg-open $(NAME).html

.PHONY: clean show
