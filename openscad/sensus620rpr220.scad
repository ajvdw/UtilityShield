/* Designed by A.J. v.d. Werken, Oct 2017    */
/* Optial sensor RPR220 - find it on Ebay    */
/* Fits on a Sensus 620 Watermeter (Vitens)  */

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
                translate([2.5,0,3]) cube([10,10,7],center=true);
            }
            // Holes
            hull()
            {
                translate([3,0,0]) cube([5.1,6.6,0.1], center=true);
                translate([3,0,7]) cube([5.3,6.8,0.1], center=true);
            }
            translate([2,-9.5,3]) cube([9,9,4], center=true);
            
            translate([-17.4,-4.6,-0.5]) cylinder(d=5.2,h=10);
            translate([17.4,-4.6,-0.5]) cylinder(d=5.2,h=10);
        }
 
    }   
}


sensus620mount();

