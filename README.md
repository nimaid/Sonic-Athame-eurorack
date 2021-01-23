# *Sonic Athame™* Modular Designs
My open-source Eurorack module designs and prototyping tools.

<div align="center"><img src="https://raw.githubusercontent.com/nimaid/Sonic-Athame-eurorack/master/Branding/Logo/logo_bg_black.png" width="400px"/></div>

## !!! Under Construction !!!
**Stay tuned as I create designs and prepare to begin selling!**

##
### **We give you the tools to direct musical energy via witchcraft!**
At Sonic Athame Modular, we see the untapped potential of the Eurorack format as a *truly* modular platform. While there are thousands of modules out there already, many of them hide away the magic of their inner workings from the musician. You put a signal in, something mysterious involving electronics happens, and a different signal comes out. Even if the musician did find the schematic for the module, they are still unable to re-configure how the internals are wired together without modifying the circuit board.

While these "integrated" modules remove complexity from patches and save space, they also inherently limit the creativity and understanding of the musician. Sonic Athame takes a different approach. We make modules that are 100% reconfigurable, similar to a semi-modular synth. Instead of one integrated module, we design sets of cleverly interconnected general-purpose "submodules". By default, these submodules are connected together to do the same thing as their integrated counterparts, but with the added benefit of being able to change how they are wired together on-the-fly to create new functionality.

Take, for example, an envelope follower. You will find most of the modules on the market have the following inputs and outputs:
* In
* Out
* Envelope Out
* Gate Out

There is nothing wrong with this way of doing things, as it makes for a small module that does it's job. However, consider an envelope follower module with the following inputs and outputs:
* In
* Out
* Full-Wave Rectifier [FWR] In (connected to `Out` when unplugged)
* FWR Out
* Low-pass Filter In (connected to `FWR Out` when unplugged)
* Low-pass Filter Out (also the `Envelope Out` if not bypassed with patch cables)
* CV Knob Out
* Comparator + In (connected to `Low-pass Filter In` when unplugged)
* Comparator - In (connected to `CV Knob Out` when unplugged)
* Comparator Out (also the `Gate Out` if not bypassed with patch cables)

When only `In`, `Out`, `Low-pass Filter Out`, and `Comparator Out` are connected, the module works like a normal envelope follower. However, if you do not need an envelope follower in your current patch, you can also use it as a utility module with:
* 1x Manual (Knob) Amplifier
* 1x Full-Wave Rectifier
* 1x Manual (Knob) Low-pass filter
* 1x CV Knob
* 1x Comparator

While this second module will take up more space in the rack than the first one due to having more input and output jacks, it is almost never going to be "dead weight". One or two of the submodules are always going to be useful, no matter what. It's even possible to combine submodules to create entirely new functionality. In essence, Sonic Athame designs modules that provide an "electronics toolkit" tailored for the job at hand, while allowing each individual "tool" to be re-used in new and creative ways.

We also design "Elemental" modules. These are essentially individual submodules from our other modules, but with all though-hole parts and a focus on cost effectiveness. These are sold as soldering kits with all the required parts, which are easy to assemble even for beginners. This allows a musician to pick and choose which utilities they want while spending the bare minimum. 