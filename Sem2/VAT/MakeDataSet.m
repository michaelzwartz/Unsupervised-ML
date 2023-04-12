function [ X ] = MakeDataSet( filename )

    Img = imread(filename);
    
    [R C] = find( Img == 0 );
    
    figure; 
    plot(R,C,'xk');
    
    X = [ R C ];

end