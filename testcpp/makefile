#
# Makefile for C++ test examples
#
CCC = g++

make:
	@make CCC=$(CCC) -C 1 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 1)
	@make CCC=$(CCC) -C 2 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 2)
	@make CCC=$(CCC) -C 3 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 3)
	@make CCC=$(CCC) -C 4 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 4)
	@make CCC=$(CCC) -C 5 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 5)
	#@make CCC=$(CCC) -C 6 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 6)
	@make CCC=$(CCC) -C 7 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 7)
	@make CCC=$(CCC) -C 8 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 8)
	@make CCC=$(CCC) -C 9 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 9)
	@make CCC=$(CCC) -C 10 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 10)
	@make CCC=$(CCC) -C 11 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 11)
	@make CCC=$(CCC) -C 12 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 12)
	@make CCC=$(CCC) -C 13 && echo "\nOK\n" || (echo "\nFAIL\n" && exit 13)

scrub:
	make -C 1 scrub
	make -C 2 scrub
	make -C 3 scrub
	make -C 4 scrub
	make -C 5 scrub
	make -C 6 scrub; make -C 6 -f makefile2 scrub
	make -C 7 scrub
	make -C 8 scrub
	make -C 9 scrub
	make -C 10 scrub
	make -C 11 scrub
	make -C 12 scrub
	make -C 13 scrub
