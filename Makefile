.PHONY: hearts deal build test disttest analyze all analyze1 analyze2

debug:
	mkdir -p debug

debug/build.ninja: debug
	cd debug && cmake -GNinja -DCMAKE_BUILD_TYPE=Debug ..

release:
	mkdir -p release

release/build.ninja: release
	cd release && cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..

hearts: debug/build.ninja
	cd debug && ninja
	./debug/hearts

opt: release/build.ninja
	cd release && ninja
	./release/hearts

deal: debug/build.ninja
	cd debug && ninja
	./debug/deal

tournament: release/build.ninja
	cd release && ninja
	./release/tournament

analyze: release/build.ninja
	cd release && ninja

analyze1: analyze
	./release/analyze -d 2006d6c0864151ba79c30127   # slam dunk shoot moon current player

analyze2: analyze
	./release/analyze -d 21b91f40fb464bc8fdda384d   # other player (2) shoots moon. May indicate bug in empirical score?

analyze3: analyze
	./release/analyze -d 5c7bd13b420f5774e65cd0b7   # Player 3 either shoots the moon or misses by 1 point

analyze4: analyze
	./release/analyze -d a5a9d06f5c340ca6ab864fb6 # Second player shoots the moon with strong, but I think stoppable hand

disttest: debug/build.ninja
	cd debug && ninja && ./disttest

validate: debug/build.ninja
	cd debug && ninja

validate1: validate
	./debug/validate 2006d6c0864151ba79c30127

all: opt tournament analyze1 disttest deal validate1
