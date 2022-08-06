package tests;

import hl.Bytes;
import thorvg.Native;
import format.png.Writer;

class Test {
    
    public static function main() {
        trace('Engine init');
        Engine.init(0);

        trace('Canvas make');
        var x = Canvas.make();

        trace('Make image');
        var image = new Array<Int>();
        image.resize(256 * 256);
        for (i in 0...(256 * 256)) {
            image[i] = 0x00000000;
        }
        
        trace('Get Array');
        var hlBytes = hl.Bytes.getArray(image);

        trace('Target');
        //x.target(hlBytes, 256, 256, 256, CS_ABGR8888_STRAIGHT);
        x.target(hlBytes, 256, 256, 256, CS_ARGB8888);
        var y = Shape.make();

        trace('Shape');

        y.moveTo(64.0, 64.0);
        y.lineTo(64.0, 192.0);
        y.lineTo(192.0, 192.0);
        y.lineTo(192.0, 64.0);
        y.close();
        y.fill(64, 128, 196, 255); // g r a b   |  a r g b | g r a b

        trace('PUsh');

        x.pushShape(y);
        y = null;
        trace('Draw');

        x.draw();
        trace('Sync');

        x.sync();

        trace('Write');

        var b = @:privateAccess new haxe.io.Bytes(hlBytes, image.length * 4);
        // Like seriously, what the fuck. the byte order is backwards?!?!?
        var d = format.png.Tools.build32BGRA(256, 256,b);

        var o = sys.io.File.write('out.png');
        var w = new format.png.Writer(o);
        
        w.write(d);
    
        Engine.destroy();
    }
}