/**
  ******************************************************************************
  * @file    README.c
  * @author  Torsten Jaekel
  * @version V1.1.0
  * @date    04-September-2016
  * @brief   introduction and overview
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 Torsten Jaekel</center></h2>
  *
  * ******************************************************************************
  *
  @mainpage LyrebirdAPP Audio Processing Platform (APP)
  @tableofcontents
  @section LyrebirdAPP Lyrebird APP System Overview
  
  The firmware supports a lightweight implementation of a <em>DANTE, Audinate</em> network audio listener and monitor for
  the audio streams in an Ethernet/IP network (similar to AVB, AES67), called <em>uDANTE</em>. It supports the standardized protocols in order to decode the audio streaming
  based on UDP, based on public protocols. It does not decode, monitor any propritary, company related intellectual property protocols.

  <em>Lyrebird APP</em> is based in a <em>STM32F796I-DISCO</em> board and uses the Ethernet connection,
  onboard DAC for Headphone, SPDIF Output etc.

  You do not need any special hardware or extension in order to monitor or listen to <em>uDNATE</em> network audio, even it is intended to be used
  on the <em>Lyrebird APP DualMCU</em> platform (as the high-professional Audio Processing Platform).

  @section LyrebirdAPP Network Audio
  <em>DANTE</em> is an Ethernet/IP network audio protocol which can be licensed from the company <em>Audinate</em>.
  It is the <em>standard<em> for Professional Network Audio where you can use Ethernet/IP switches for the audio streaming.

  The audio is transmitted as UPD packets, with a fix length (e.g. 297 bytes per UDP packet, 1 byte for channel number, 8 byte for
  audio time stamp, all other payload as audio samples 24-bit, big endian).

  The <em>Lyrebird APP</em> is capable to monitor such a network, figure out where the UDP audio is running, to connect to
  a running audio stream and decode, forward it to other interfaces, such as headphone out, SPDIF Out, I2S out.

  All the protocols related to Device Discovery, Audio Routing, Announcements etc. is not (yet) supported. The implementation of the
  <em>Audinate DANTE</em> is based on the public standards and implementation for network data/audio reception.

  <em>Lyrebird APP</em> uses the <em>FreeRTOS</em> and <em>LwIP</em> in order to monitor and listen to <em>Audinate DANTE</em> network
  audio streams.
  
  <em>Lyrebird APP</em> is developed for STM32F7 board, e.g. STM232F769I-DISCO, in combination with ARM GNU GCC, AC6/SystemWorkbench
  and does not need any additional hardware to run.

  @section LyrebirdAPPMonitor Monitoring

  You need a running <em>Audinate DANTE</em> audio network. A player has to send audio via Ethernet/IP networks to an audio sink. <em>Lyrebird APP</em>
  can monitor (a passive listener), decode and play the audio running in such a network. It will not (et) act as full compliant participant in a
  <em>DANTE</em> network. If so in future, it will be based on a legal license, it might be implemented completely on public network
  standards.

  Without a <em>DANTE</em> receiver it will not work. <em>Lyrebird APP</em> is not (yet) a full compliant participant in such an audio network.
  As a player you can use <em>DANTE Virtual Soundcard (DVS)</em>.

  <em>Lyrebird APP</em> will find the audio stream in a <em>DANTE</em> audio network, opens the connection, gathers and decodes the audio,
  forwards the audio to a different output interface (e.g. SPDIF, I2S). It does not act as a clock device, not as an ingress device (to insert audio into network).

  @section Remarks Remarks and important information

  <em>Lyrebird APP</em> supports the subset of Network Audio for the public standard based audio streaming (IP/UDP). It does not support or implements
  network protocols which are licensed or proprietary, an intellectual properties of other companies.

  You can make use of the provided firmware without any warranty or expected maintenance, no future bug fixes or notifications about
  changes, improvements etc.
  */
