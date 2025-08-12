#!/bin/sh

MODULES_DIR=board/nuvoton/ma35h0/modules/5.10.140
MODULES_TDIR=$TARGET_DIR/lib/modules/5.10.140
GFXDRIVERS_TDIR=$TARGET_DIR/usr/lib/directfb-1.7-7/gfxdrivers

RESIZE_FILE=${TARGET_DIR}/etc/init.d/S50resize
cp $MODULES_DIR/../../resize.sh ${TARGET_DIR}/etc/
if [ -f ${RESIZE_FILE} ]; then
        rm ${RESIZE_FILE}
fi
if grep -Eq "^BR2_MA35H0_RESIZE_SD_MAX=y$" ${BR2_CONFIG}; then
	export $(grep "BR2_MA35H0_RESIZE_DISK_DRIVE=" $BR2_CONFIG | sed 's/\"//g')
	export $(grep "BR2_MA35H0_RESIZE_DISK_NUM=" $BR2_CONFIG | sed 's/\"//g')
	echo "#!/bin/sh" >> ${RESIZE_FILE}
	echo "/etc/resize.sh ${BR2_MA35H0_RESIZE_DISK_DRIVE} ${BR2_MA35H0_RESIZE_DISK_NUM}" >> ${RESIZE_FILE}
	chmod 755 ${RESIZE_FILE}
fi

if grep -Eq "^BR2_PACKAGE_BUSYBOX=y$" ${BR2_CONFIG}; then
	install -d -m 755 ${MODULES_TARGET_TDIR}
	install -d -m 755 ${GFXDRIVERS_TDIR}
	cp ${MODULES_DIR}/*.ko ${MODULES_TDIR}/
	cp ${MODULES_DIR}/../libdirectfb_gal.so ${GFXDRIVERS_TDIR}/
	cp ${MODULES_DIR}/../libGAL.so ${TARGET_DIR}/usr/lib/
	cp ${MODULES_DIR}/../modules.sh ${TARGET_DIR}/etc/profile.d/
fi

OVERLAY_DIR=./overlay

GUIDEMO_DIR=./custom/MA35_Family_Linux_emWin_64bit_Package_20240618/Sample/GUIDemo
SIMPLEDEMO_DIR=./custom/MA35_Family_Linux_emWin_64bit_Package_20240618/Sample/SimpleDemo
SIMPLEDEMOAPPWIZARD_DIR=./custom/MA35_Family_Linux_emWin_64bit_Package_20240618/Sample/SimpleDemoAppWizard

make -C $GUIDEMO_DIR
if [ -f "$GUIDEMO_DIR/GUIDemo" ]; then
	chmod -R 777 $GUIDEMO_DIR/GUIDemo
	cp $GUIDEMO_DIR/GUIDemo $OVERLAY_DIR/user_data/GUIDemo
fi

make -C $SIMPLEDEMO_DIR
if [ -f "$SIMPLEDEMO_DIR/SimpleDemo" ]; then
	chmod -R 777 $SIMPLEDEMO_DIR/SimpleDemo
        cp $SIMPLEDEMO_DIR/SimpleDemo $OVERLAY_DIR/user_data/SimpleDemo
fi

make -C $SIMPLEDEMOAPPWIZARD_DIR
if [ -f "$SIMPLEDEMOAPPWIZARD_DIR/SimpleDemoAppWizard" ]; then
	chmod -R 777 $SIMPLEDEMOAPPWIZARD_DIR/SimpleDemoAppWizard
        cp $SIMPLEDEMOAPPWIZARD_DIR/SimpleDemoAppWizard $OVERLAY_DIR/user_data/SimpleDemoAppWizard
fi

rsync -a --exclude='.empty' --exclude='*~' $OVERLAY_DIR/ $TARGET_DIR/

echo "Overlays applied after post-build."
