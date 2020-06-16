.. _overrall:

Overrall
#########

**Zephyr Functional Safety Certification**

This is a software architecture design test specification for zephyr RTOS prject. This document provides a generic verification specification suitable for use in IEC 61508 and ISO 26262 compliant developments.

**Acronyms and Terminology**:
	
+------------+--------------------------+
| Term       | Description              |
+============+==========================+
| RTOS       | real time operate system |
+------------+--------------------------+
|            |                          |
+------------+--------------------------+
	

**Revision History**:

+------------+------------+------------+-----------------------+
| Data       | Revision   | Auther     | Description           |
+============+============+============+=======================+
| 2020/06/19 | 1.0        | Zhu Youhua | Initial draft version.|
+------------+------------+------------+-----------------------+
|            |            |            |                       |
+------------+------------+------------+-----------------------+

**Applicable Standards**:

+----------------------------------+-----------------------------------------+
| Document                         | Document Number / Location              |
+==================================+=========================================+
|IEC 61508-2:2010, Functional      |                                         |
|safety of electrical/electronic/  |                                         |
|programmable electronic           |                                         |                                        
|safety-related-systems – Part 2:  |         www.iec.ch                      |
|Requirements for electrical/      |                                         |
|electronic/programmable electronic|                                         |
|safety-related systems	           |                                         |
+----------------------------------+-----------------------------------------+
|IEC 61508-3:2010,Functional safety|                                         |
|of electrical/electronic/         |                                         |
|programmable electronic           |                                         |                                        
|safety-related-systems – Part 3:  |         www.iec.ch                      |
|Software requirements             |                                         |
+----------------------------------+-----------------------------------------+	
|ISO 26262-8:2011, Road vehicles — |                                         |
|Functional safety — Part 8:       |          www.iso.org                    |
|Supporting processes              |                                         |                                        
+----------------------------------+-----------------------------------------+		

**Reference Documents**:
    https://zephyrproject.org/

::

    No license (express or implied, by estoppel or otherwise) to any intellectual property rights is granted by this document.
    All information provided here is subject to change without notice. Contact your Intel representative to obtain the latest Intel product specifications and roadmaps.
    The products described may contain design defects or errors known as errata which may cause the product to deviate from published specifications. Current characterized errata are available on request.
    Copies of documents which have an order number and are referenced in this document may be obtained by calling 1-800-548-4725 or by visiting:  http://www.intel.com/design/literature.htm 
    Intel technologies' features and benefits depend on system configuration and may require enabled hardware, software or service activation. Performance varies depending on system configuration. No computer system can be absolutely secure. Check with your system manufacturer or retailer or learn more at [intel.com].
    This material may relate to the creation of end products used in safety-critical applications designed to comply with functional safety standards or requirements (“Safety-Critical Applications”).  You agree and represent that you have all the necessary expertise to design, manage and ensure effective system-level safeguards to anticipate, monitor and control system failures in safety-critical applications. It is your sole responsibility to design, manage and assure system-level safeguards to anticipate, monitor and control system failures, and you agree that you are solely responsible for all applicable regulatory standards and safety-related requirements concerning your use of any material related to Safety Critical Applications.  You agree to indemnify and hold Intel and its representatives harmless against any damages, costs, and expenses arising in any way out of your use of the material related to Safety-Critical Applications.  You further agree that some of the material maybe be pre-production in nature and that all material is provided "as is" without any express or implied warranty of any kind including warranties of merchantability, noninfringement, or fitness for a particular purpose. intel does not warrant or assume responsibility for the accuracy or completeness of any material provided.
    Intel, Intel brands, and the Intel logo are trademarks of Intel Corporation in the U.S. and/or other countries.
    *Other names and brands may be claimed as the property of others.	
    Copyright © 2020, Intel Corporation. All rights reserved.
