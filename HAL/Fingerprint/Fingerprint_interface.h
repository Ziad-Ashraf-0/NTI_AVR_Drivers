/*
 * Fingerprint_interface.h
 *
 * Created: 11/4/2023 9:32:18 AM
 *  Author: Ziad
 */ 


#ifndef FINGERPRINT_INTERFACE_H_
#define FINGERPRINT_INTERFACE_H_


u8 FingerPS_strTemplate(u8 bufferId, u16 pageId);
u8 FingerPS_searchFinger(u8 bufferId, u16 startPage, u16 pageNum);


#endif /* FINGERPRINT_INTERFACE_H_ */