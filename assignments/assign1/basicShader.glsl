#version 130 

uniform vec4 eye;
uniform vec4 ambient;
uniform vec4[20] objects;
uniform vec4[20] objColors;
uniform vec4[10] lightsDirection;
uniform vec4[10] lightsIntensity;
uniform vec4[10] lightPosition;
uniform ivec3 sizes; //number of objects & number of lights
uniform vec4 ambient1 = vec4(0.1,0.2,0.3,1.0);
in vec3 position1;


vec3 findN(vec4 object, vec3 interPoint){
	vec3 N ;
	vec3 tmp;
	if(object.w > 0.0){
		tmp.xyz = interPoint.xyz - object.xyz;
		N = -tmp/object.w;
	}
	else{
		N = object.xyz;
	}
	return normalize(N.xyz);
}


float sphereIntersection(vec3 sphere,vec3 vec, float r, vec3 sourcePoint)
{	
	float t = -1.0;
	float a = 1.0;
	float b =dot(vec,sourcePoint-sphere.xyz);
	float c = -r*r+dot(sourcePoint-sphere.xyz, sourcePoint-sphere.xyz);
	if(b*b-a*c< 0.0){
		return t;
	}
	float x1= -b-sqrt(b*b-a*c);
	float x2= -b+sqrt(b*b-a*c);

	float res = min(x1,x2);
	return res;
}


float planeIntersection(vec3 object, float w, vec3 sourcePoint)
{
    vec3 N = object.xyz;
	vec3 vec = normalize (position1.xyz - sourcePoint);
	float cos = dot(normalize(N), vec);
	vec3 tmpv ;
	if (object.x != 0.0){
		return dot (N,((vec3 (-w / object.x, 0.0,  0.0) - sourcePoint) / cos));

	}


	else if (object.y != 0.0){
		return dot (N,((vec3 ( 0.0, -w / object.y,  0.0) - sourcePoint) / cos));
	}

	else{
		return dot (N,((vec3 ( 0.0,  0.0, -w / object.z) - sourcePoint) / cos));
	}

	
	
	
}


vec3 findInterPoint(vec3 vec,float t){ //finds where camera vector intersect minobject

	vec3 p;
	vec3 tv = vec3(t*vec.xyz);
	p = eye.xyz + tv;
	return p;

}

float intersection(vec3 object,vec3 v, float w , vec3 sourcePoint)
{
	float distance =-1.0;

    if( w>0.0){				//check if plane or sphere
    	distance =sphereIntersection(object,v,w,sourcePoint);
    }
    else{
    	distance = planeIntersection(object,w,sourcePoint);
    }
    return distance;				//calc distance from object to pixel
    
}



bool check_quart(vec3 interPoint){

	return (interPoint.x>0.0 && interPoint.y>0.0) || (interPoint.x<0.0 && interPoint.y<0.0) ;
}

vec3 calcDiffuseAndSpecular(int j,int index, vec3 interPoint, vec3 Kd){

	vec3 tmpcolor;
	vec3 N = normalize(findN(objects[index],interPoint));
	vec3 L;
	if(lightsDirection[j].w == 1.0){
		L = normalize(interPoint - lightPosition[j].xyz);
	}
	else{
		L = normalize(lightsDirection[j].xyz);
	}


	// ******Diffuse calculation*******
	vec3 D = normalize(lightsDirection[j].xyz);
	float cosA = dot(N,L)/(length(N)*length(L));
	vec3 DdotL = vec3(dot(D,L)); 
	vec3 IL= lightsIntensity[j].xyz *(DdotL);
	vec3 ldiffuse = Kd * cosA * IL;
			

	//********* Specular calculation************
	vec3 vec = normalize(position1 - eye.xyz);
	vec4 Ks = vec4(0.7,0.7,0.7,1.0);
	vec3 R = normalize(reflect(L,N));
	float n = objColors[index].w;
	vec3 lspecular = (Ks.xyz * pow(dot(R,vec),n)*(IL));
	tmpcolor = lspecular+ldiffuse;
	return tmpcolor;

}

vec3 colorCalc( vec3 intersectionPoint)
{
	vec3 interPoint;
	vec3 Kd;
	vec3 color;
	vec3 ambient = vec3(0,0,0);
	vec3 diffuse= vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	float minObject = 100000.0; 	 //saves the closest object to curr pixel
	int index = -1;  		   //index of minObject
	float ans = -1.0;		   //colsest object intersect
	vec3 vec = normalize(position1 - intersectionPoint);    //vec from camera
	for(int i =0; i<sizes.x;i++){    //find closest objest
		ans = intersection(objects[i].xyz,vec, objects[i].w,eye.xyz);   //find intersection between object to vec of camera
		if(ans > 0.001 && ans < minObject){
			minObject = ans;
			index = i;
		}
	}               
	if (minObject == 100000.0){        //object intersect not found
    	color = vec3(0,0,0);
		return color;	        //pixel doesnt intersect
    	
	}

	interPoint = findInterPoint(vec,minObject);
	bool modulo= (mod(int(1.5*interPoint.x),2)==mod(int(1.5*interPoint.y),2));
    if(objects[index].w<0.0 && (( modulo && check_quart(interPoint)) || !( modulo|| check_quart(interPoint)))){
		Kd = vec3(0.5,0.5,0.5);
	}
	else{
		Kd = objColors[index].xyz;
	}

	// * Ambient calculation***********
	color=ambient.xyz*objColors[index].xyz;

    for (int j=0; j<sizes.y; j++){
    		//check if there is shadow
    		float ans2;
    		int index2;
	    	vec3 L;
	    	float minobject2 = 100000.0;
    	for(int k=0; k<sizes.x;k++){//go throw all objects and check if the light is blocked by any of them
    		
    			if (k!=index){
	    		if(lightsDirection[j].w == 0.0){
	    			L = normalize(-lightsDirection[j].xyz);
	    		}
	    		else{
	    			L = normalize(lightPosition[j].xyz-interPoint);
	    		}
	    		ans2 =intersection(objects[k].xyz,L, objects[k].w,interPoint);
	    		if(ans2 > 0.001 && ans2 < minobject2){
					minobject2 = ans2;
					index2 = k;

				}
				}
			}
				if(minobject2 >0 && minobject2<100000.0){//if we found intersection between an object and the light ray we need to check if it blocks the original object
					
					//vec3 tmp = normalize(lightPosition[j].xyz - objects[index2].xyz);
					vec3 tmp = L;
					vec3 newinterPoint = interPoint + minobject2 *tmp;
					vec3 a = lightPosition[j].xyz - interPoint;
					vec3 b = newinterPoint - interPoint;
					
					//vec3 t=objects[index2].xyz +minObject*tmp;
					if(length(b) > length(a)){
						minobject2=100000.0;
					}
				}
				if(minobject2 == 100000.0){

					if(lightsDirection[j].w == 0.0){
					color += calcDiffuseAndSpecular(j,index, interPoint, Kd);
					}
					else{
						
	   					if(dot(-L,normalize(lightsDirection[j].xyz))> lightPosition[j].w){
	   						color += calcDiffuseAndSpecular(j,index, interPoint, Kd);
	   					}
					}
		   		
	   	}	
    	
    }
    
	
	color.x = min(1.0, color.x);
    color.y = min(1.0, color.y);
    color.z = min(1.0, color.z);

    color.x = max(0.0, color.x);
    color.y = max(0.0, color.y);
    color.z = max(0.0, color.z);

    return color;
}
	

void main()
{  
   
            gl_FragColor = vec4(colorCalc(eye.xyz),1);      
}#version 130 

uniform vec4 eye;
uniform vec4 ambient;
uniform vec4[20] objects;
uniform vec4[20] objColors;
uniform vec4[10] lightsDirection;
uniform vec4[10] lightsIntensity;
uniform vec4[10] lightPosition;
uniform ivec3 sizes; //number of objects & number of lights
uniform vec4 ambient1 = vec4(0.1,0.2,0.3,1.0);
in vec3 position1;


vec3 findN(vec4 object, vec3 interPoint){
	vec3 N ;
	vec3 tmp;
	if(object.w > 0.0){
		tmp.xyz = interPoint.xyz - object.xyz;
		N = -tmp/object.w;
	}
	else{
		N = object.xyz;
	}
	return normalize(N.xyz);
}


float sphereIntersection(vec3 sphere,vec3 vec, float r, vec3 sourcePoint)
{	
	float t = -1.0;
	float a = 1.0;
	float b =dot(vec,sourcePoint-sphere.xyz);
	float c = -r*r+dot(sourcePoint-sphere.xyz, sourcePoint-sphere.xyz);
	if(b*b-a*c< 0.0){
		return t;
	}
	float x1= -b-sqrt(b*b-a*c);
	float x2= -b+sqrt(b*b-a*c);

	float res = min(x1,x2);
	return res;
}


float planeIntersection(vec3 object, float w, vec3 sourcePoint)
{
    vec3 N = object.xyz;
	vec3 vec = normalize (position1.xyz - sourcePoint);
	float cos = dot(normalize(N), vec);
	vec3 tmpv ;
	if (object.x != 0.0){
		return dot (N,((vec3 (-w / object.x, 0.0,  0.0) - sourcePoint) / cos));

	}


	else if (object.y != 0.0){
		return dot (N,((vec3 ( 0.0, -w / object.y,  0.0) - sourcePoint) / cos));
	}

	else{
		return dot (N,((vec3 ( 0.0,  0.0, -w / object.z) - sourcePoint) / cos));
	}

	
	
	
}


vec3 findInterPoint(vec3 vec,float t){ //finds where camera vector intersect minobject

	vec3 p;
	vec3 tv = vec3(t*vec.xyz);
	p = eye.xyz + tv;
	return p;

}

float intersection(vec3 object,vec3 v, float w , vec3 sourcePoint)
{
	float distance =-1.0;

    if( w>0.0){				//check if plane or sphere
    	distance =sphereIntersection(object,v,w,sourcePoint);
    }
    else{
    	distance = planeIntersection(object,w,sourcePoint);
    }
    return distance;				//calc distance from object to pixel
    
}



bool check_quart(vec3 interPoint){

	return (interPoint.x>0.0 && interPoint.y>0.0) || (interPoint.x<0.0 && interPoint.y<0.0) ;
}

vec3 calcDiffuseAndSpecular(int j,int index, vec3 interPoint, vec3 Kd){

	vec3 tmpcolor;
	vec3 N = normalize(findN(objects[index],interPoint));
	vec3 L;
	if(lightsDirection[j].w == 1.0){
		L = normalize(interPoint - lightPosition[j].xyz);
	}
	else{
		L = normalize(lightsDirection[j].xyz);
	}


	// ******Diffuse calculation*******
	vec3 D = normalize(lightsDirection[j].xyz);
	float cosA = dot(N,L)/(length(N)*length(L));
	vec3 DdotL = vec3(dot(D,L)); 
	vec3 IL= lightsIntensity[j].xyz *(DdotL);
	vec3 ldiffuse = Kd * cosA * IL;
			

	//********* Specular calculation************
	vec3 vec = normalize(position1 - eye.xyz);
	vec4 Ks = vec4(0.7,0.7,0.7,1.0);
	vec3 R = normalize(reflect(L,N));
	float n = objColors[index].w;
	vec3 lspecular = (Ks.xyz * pow(dot(R,vec),n)*(IL));
	tmpcolor = lspecular+ldiffuse;
	return tmpcolor;

}

vec3 colorCalc( vec3 intersectionPoint)
{
	vec3 interPoint;
	vec3 Kd;
	vec3 color;
	vec3 ambient = vec3(0,0,0);
	vec3 diffuse= vec3(0,0,0);
	vec3 specular = vec3(0,0,0);
	float minObject = 100000.0; 	 //saves the closest object to curr pixel
	int index = -1;  		   //index of minObject
	float ans = -1.0;		   //colsest object intersect
	vec3 vec = normalize(position1 - intersectionPoint);    //vec from camera
	for(int i =0; i<sizes.x;i++){    //find closest objest
		ans = intersection(objects[i].xyz,vec, objects[i].w,eye.xyz);   //find intersection between object to vec of camera
		if(ans > 0.001 && ans < minObject){
			minObject = ans;
			index = i;
		}
	}               
	if (minObject == 100000.0){        //object intersect not found
    	color = vec3(0,0,0);
		return color;	        //pixel doesnt intersect
    	
	}

	interPoint = findInterPoint(vec,minObject);
	bool modulo= (mod(int(1.5*interPoint.x),2)==mod(int(1.5*interPoint.y),2));
    if(objects[index].w<0.0 && (( modulo && check_quart(interPoint)) || !( modulo|| check_quart(interPoint)))){
		Kd = vec3(0.5,0.5,0.5);
	}
	else{
		Kd = objColors[index].xyz;
	}

	// * Ambient calculation***********
	color=ambient.xyz*objColors[index].xyz;

    for (int j=0; j<sizes.y; j++){
    		//check if there is shadow
    		float ans2;
    		int index2;
	    	vec3 L;
	    	float minobject2 = 100000.0;
    	for(int k=0; k<sizes.x;k++){//go throw all objects and check if the light is blocked by any of them
    		
    			if (k!=index){
	    		if(lightsDirection[j].w == 0.0){
	    			L = normalize(-lightsDirection[j].xyz);
	    		}
	    		else{
	    			L = normalize(lightPosition[j].xyz-interPoint);
	    		}
	    		ans2 =intersection(objects[k].xyz,L, objects[k].w,interPoint);
	    		if(ans2 > 0.001 && ans2 < minobject2){
					minobject2 = ans2;
					index2 = k;

				}
				}
			}
				if(minobject2 >0 && minobject2<100000.0){//if we found intersection between an object and the light ray we need to check if it blocks the original object
					
					//vec3 tmp = normalize(lightPosition[j].xyz - objects[index2].xyz);
					vec3 tmp = L;
					vec3 newinterPoint = interPoint + minobject2 *tmp;
					vec3 a = lightPosition[j].xyz - interPoint;
					vec3 b = newinterPoint - interPoint;
					
					//vec3 t=objects[index2].xyz +minObject*tmp;
					if(length(b) > length(a)){
						minobject2=100000.0;
					}
				}
				if(minobject2 == 100000.0){

					if(lightsDirection[j].w == 0.0){
					color += calcDiffuseAndSpecular(j,index, interPoint, Kd);
					}
					else{
						
	   					if(dot(-L,normalize(lightsDirection[j].xyz))> lightPosition[j].w){
	   						color += calcDiffuseAndSpecular(j,index, interPoint, Kd);
	   					}
					}
		   		
	   	}	
    	
    }
    
	
	color.x = min(1.0, color.x);
    color.y = min(1.0, color.y);
    color.z = min(1.0, color.z);

    color.x = max(0.0, color.x);
    color.y = max(0.0, color.y);
    color.z = max(0.0, color.z);

    return color;
}
	

void main()
{  
   
            gl_FragColor = vec4(colorCalc(eye.xyz),1);      
}