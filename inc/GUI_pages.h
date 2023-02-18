/**
  ******************************************************************************
  * @file    GUI_pages.h
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   The definition for the BMP files stored on QSPI flash device
  ******************************************************************************
  */

#ifndef GUI_PAGES_H_
#define GUI_PAGES_H_

/*
 * define how to access the GUI pages in QSPI flash with memory remapping
 * a BMP file as real binary for LCD size 272 x 480 Pixel is stored in QSPI, we
 * do not need anymore page_x.h for the C-Code, instead we had to use debugger ST-LINK
 * flashing a BIN file with all the pages (see separate instruction how to do)
 */

#define	QSPI_MEM_BASE		0x90000000
#define QSPI_SRVD_CONFIG	0x00001000	/* we reserve 4K at beginning of QSPI for config */

/*
 * Remark: we make sure to align the pages on a 4K boundary: so we could use sector erase,
 * it waste a bit space in QSPI (holes)
 * For the holes make sure to define as 0xFF in BIN file (so we could still override without to
 * erase)
 */

#define	BMP_FILE_SIZE		0x00120000		/* a bit more as real file size for 800x480, round up to next boundary */

#define	PAGE_SPLASH			(QSPI_MEM_BASE + QSPI_SRVD_CONFIG)							/* page0 - Splash screen */
#define	PAGE_SD				(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 1 * BMP_FILE_SIZE)		/* page1 - Main page */
#define	PAGE_CONFIG			(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 2 * BMP_FILE_SIZE)		/* page2 - Config */
#define	PAGE_3				(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 3 * BMP_FILE_SIZE)		/* page3 - Status */
#define	PAGE_4				(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 4 * BMP_FILE_SIZE)		/* page4 - FFT */
#define	PAGE_5				(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 5 * BMP_FILE_SIZE)		/* page5 - Filter (EQ) */
#define	PAGE_6				(QSPI_MEM_BASE + QSPI_SRVD_CONFIG + 6 * BMP_FILE_SIZE)		/* page6 - SDCard files */

/*
 * Remark: we have a total QSPI size of 512Mbit = 64MByte = 0x9000_0000 .. 0x9400_0000
 */


#endif /* GUI_PAGES_H_ */
