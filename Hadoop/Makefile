JAR         = /opt/cloudera/parcels/CDH-5.4.2-1.cdh5.4.2.p0.2/lib/hadoop-0.20-mapreduce/contrib/streaming/hadoop-streaming-2.6.0-mr1-cdh5.4.2.jar
HADOOP_USER = vagrant
DATA_INPUT  = /user/$(HADOOP_USER)/input
DATA_OUTPUT = /user/$(HADOOP_USER)/output

clean:
	@echo "cleaning database"
	hadoop fs -rm -f -r $(DATA_INPUT)
	hadoop fs -rm -f -r $(DATA_OUTPUT)
	hadoop fs -mkdir $(DATA_INPUT)

upload: 2007.csv 2008.csv clean
	@echo "uploading database"
	hadoop fs -copyFromLocal 2007.csv $(DATA_INPUT)/
	hadoop fs -copyFromLocal 2008.csv $(DATA_INPUT)/

exe:
	@echo "executing"
	hadoop jar $(JAR) \
        -file mapper.py    -mapper mapper.py \
        -file reducer.py   -reducer reducer.py \
        -input $(DATA_INPUT) \
        -output $(DATA_OUTPUT) \
        -numReduceTasks 1
