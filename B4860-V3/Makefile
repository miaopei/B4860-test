MAKE = make

BBU = bbu
HUB = hub
RRU = rru

all: 
	@echo "make all ..."
	cd $(BBU) && $(MAKE) && cd -
	cd $(HUB) && $(MAKE) && cd -
	cd $(RRU) && $(MAKE) && cd -

.PHONY: clean
clean:
	cd $(BBU) && $(MAKE) clean && cd -	
	cd $(HUB) && $(MAKE) clean && cd -	
	cd $(RRU) && $(MAKE) clean && cd -	

