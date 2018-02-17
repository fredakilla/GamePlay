QMAKE_CLEAN += $$DESTDIR/$$TARGET

TEMPLATE = subdirs

CONFIG = ordered

SUBDIRS += \
    gameplay/gameplay.pro \
    samples/browser/sample-browser.pro \
    #samples/character/sample-character.pro \
    #samples/spaceship/sample-spaceship.pro \
    #samples/racer/sample-racer.pro \



