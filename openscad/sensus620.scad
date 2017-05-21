/* Designed by A.J. v.d. Werken, May 2017 */

$fn=90;

module sensus620mount()
{
    intersection()
    {
        cylinder(d=57,h=25);
        difference()
        {
            union()
            {
                hull()
                {
                    translate([-17.4,11.2,0]) cylinder(d=16,h=3);
                    translate([17.4,11.2,0]) cylinder(d=16,h=3);
                    translate([3,35.8,-0.5]) cylinder(d=20,h=3.5);
                }
                translate([-17.4,11.2,0]) cylinder(d=11,h=7);
                translate([17.4,11.2,0]) cylinder(d=11,h=7);
                translate([2,16.8,7.5]) cube([20,25,15],center=true);
            }
            // Holes
            translate([-17.4,11.2,-0.5]) cylinder(d=5.2,h=10);
            translate([17.4,11.2,-0.5]) cylinder(d=5.2,h=10);
            translate([3,15.8,-0.5]) cylinder(d=10.3,h=4);
            translate([3,15.8,2.95]) cylinder(d=12.5,h=18);
            translate([1.05,15.8,8.5]) cube([22,17,5],center=true);
        }
 
    }   
}


sensus620mount();

