if ! [ -d "/tmp/ns" ]; then
	cd /tmp/
	yaourt -G ns
fi
sh -c "cd ~/wsn/ns-2.35/diffusion3/apps/ping; echo Start Coding; zsh"
echo "Start Building"

if [ "$1" != "try" ]; then
	cd ~/wsn
	tar czf ns-2.35.tar.gz ns-2.35
	cd /tmp/ns

	mv ~/wsn/ns-2.35.tar.gz ./ns-src-2.35.tar.gz
	if makepkg -f --skipchecksums 2>&1 > /tmp/buildlog ; then
		makepkg -i
	fi
else
	cd /tmp/ns
	echo "'./workflow final' guarantees the build result, but very slow"
	rsync -avr ~/wsn/ns-2.35 ./src/
	cd ./src/ns-2.35
	if make; then
		cd ../../
		makepkg -i
	fi
fi
