/* Designed by A.J. v.d. Werken, May 2017           */
/* Inductive Proximity Sensor Switch NPN DC 6V-36V  */
/* LJ12A3-4-Z/BX - find it on Ebay for approx. $2   */
/* Fits on a Sensus 620 Watermeter (Vitens)         */

$fn=180;

module sensus620mount()
{
    intersection()
    {
        translate([0,-15.8,0] ) cylinder(d=57,h=25);
        difference()
        {
            union()
            {
                hull()
                {
                    translate([-17.4,-4.6,0]) cylinder(d=16,h=3);
                    translate([17.4,-4.6,0]) cylinder(d=16,h=3);
                    translate([3,20,-0.5]) cylinder(d=20,h=3.5);
                }
                translate([-17.4,-4.6,0]) cylinder(d=11,h=7);
                translate([17.4,-4.6,0]) cylinder(d=11,h=7);
                translate([0,0.1,7.5]) cube([23.6,25.4,15],center=true);
            }
            // Holes
            translate([-17.4,-4.6,-0.5]) cylinder(d=5.2,h=10);
            translate([17.4,-4.6,-0.5]) cylinder(d=5.2,h=10);
            translate([3,0,-0.5]) cylinder(d=10.3,h=4);
            translate([3,0,2.95]) cylinder(d=12.5,h=18);
            translate([1.05,0,8.5]) cube([22,17,5],center=true);
        }
 
    }   
}


sensus620mount();

